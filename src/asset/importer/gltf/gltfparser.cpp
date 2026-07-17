#include "asset/importer/gltf/gltfparser.h"

#include "core/debug/debugger.h"

#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

// =========================================================================
// Base64 decoding table
// =========================================================================
static const char kBase64Table[256] = {
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,62,-1,-1,-1,63,
    52,53,54,55,56,57,58,59, 60,61,-1,-1,-1,-1,-1,-1,
    -1, 0, 1, 2, 3, 4, 5, 6,  7, 8, 9,10,11,12,13,14,
    15,16,17,18,19,20,21,22, 23,24,25,-1,-1,-1,-1,-1,
    -1,26,27,28,29,30,31,32, 33,34,35,36,37,38,39,40,
    41,42,43,44,45,46,47,48, 49,50,51,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
};

// =========================================================================
// Helpers
// =========================================================================
std::string GltfParser::dirName(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return ".";
    return path.substr(0, pos);
}

// =========================================================================
// Base64 decoder (supports standard + URL-safe alphabet, with or without padding)
// =========================================================================
std::vector<u8> GltfParser::decodeBase64(const std::string& encoded) const {
    std::vector<u8> out;
    out.reserve((encoded.size() * 3) / 4);

    int val = 0;
    int bits = -8; // sentinel: buffer empty

    for (size_t i = 0; i < encoded.size(); ++i) {
        char c = encoded[i];
        if (c == '=' || c == '\n' || c == '\r' || c == ' ') continue;

        char idx = kBase64Table[static_cast<u8>(c)];
        if (idx == -1) continue;

        val = (val << 6) | idx;
        bits += 6;

        if (bits >= 0) {
            out.push_back(static_cast<u8>((val >> bits) & 0xFF));
            bits -= 8;
        }
    }
    return out;
}

// =========================================================================
// Component type → byte size
// =========================================================================
u32 GltfParser::getComponentSize(u32 componentType) const {
    switch (componentType) {
    case 5120: return 1; // BYTE
    case 5121: return 1; // UNSIGNED_BYTE
    case 5122: return 2; // SHORT
    case 5123: return 2; // UNSIGNED_SHORT
    case 5125: return 4; // UNSIGNED_INT
    case 5126: return 4; // FLOAT
    default:
        WARNING("Unknown glTF componentType: {} — assuming 4 bytes", componentType);
        return 4;
    }
}

// =========================================================================
// Accessor type → number of components
// =========================================================================
u32 GltfParser::getTypeCount(const std::string& type) const {
    if (type == "SCALAR") return 1;
    if (type == "VEC2")   return 2;
    if (type == "VEC3")   return 3;
    if (type == "VEC4")   return 4;
    if (type == "MAT2")   return 4;
    if (type == "MAT3")   return 9;
    if (type == "MAT4")   return 16;
    WARNING("Unknown glTF accessor type: {} — assuming 1", type.c_str());
    return 1;
}

// =========================================================================
// Lifecycle
// =========================================================================
void GltfParser::init() {
    m_root     = nullptr;
    m_currMesh = nullptr;
}

void GltfParser::destroy() {
    m_buffers.clear();
    m_root     = nullptr;
    m_currMesh = nullptr;
}

// =========================================================================
// Main entry point
// =========================================================================
void GltfParser::parse(const char* path, Mesh* pMesh) {
    if (!path || !pMesh) {
        ERROR("GltfParser::parse — null path or mesh pointer");
        return;
    }
    m_currMesh = pMesh;

    // ---- clear previous mesh data ------------------------------------------
    m_currMesh->positions().clear();
    m_currMesh->texcoords().clear();
    m_currMesh->normals().clear();
    m_currMesh->vertices().clear();
    m_currMesh->indices().clear();

    m_buffers.clear();

    // ---- read file ----------------------------------------------------------
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        ERROR("Failed to open glTF file: {}", path);
        return;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    std::string jsonStr = ss.str();

    // ---- parse JSON ---------------------------------------------------------
    gltf_json::JsonValue root = gltf_json::parse(jsonStr);
    if (!root.isObject()) {
        ERROR("glTF root is not a JSON object");
        return;
    }
    m_root = &root;

    // ---- validate asset version --------------------------------------------
    if (root.has("asset")) {
        const auto& asset = root["asset"];
        if (asset.has("version")) {
            const std::string& ver = asset["version"].getString();
            if (ver[0] != '2') {
                WARNING("glTF version {} — parser targets 2.0, may be incompatible", ver.c_str());
            }
        }
    }

    // ---- load buffers -------------------------------------------------------
    std::string gltfDir = dirName(path);
    loadBuffers(gltfDir, root);

    // ---- traverse scene → nodes → meshes -----------------------------------
    if (!root.has("scenes") || !root.has("nodes") || !root.has("meshes")) {
        ERROR("glTF file missing required top-level arrays (scenes/nodes/meshes)");
        return;
    }

    const auto& scenes = root["scenes"];
    const auto& nodes  = root["nodes"];
    const auto& meshes = root["meshes"];

    // determine which scene to load
    int sceneIdx = 0;
    if (root.has("scene")) {
        sceneIdx = root["scene"].getInt();
    }

    if (sceneIdx < 0 || static_cast<size_t>(sceneIdx) >= scenes.size()) {
        ERROR("glTF default scene index {} out of range (0..{})", sceneIdx, scenes.size() - 1);
        return;
    }

    const auto& scene = scenes[static_cast<size_t>(sceneIdx)];
    if (!scene.has("nodes")) {
        ERROR("glTF scene[{}] has no nodes array", sceneIdx);
        return;
    }

    const auto& sceneNodes = scene["nodes"];

    // walk every root node, recursively collect meshes
    for (size_t ni = 0; ni < sceneNodes.size(); ++ni) {
        int nodeIdx = sceneNodes[static_cast<size_t>(ni)].getInt();

        if (nodeIdx < 0 || static_cast<size_t>(nodeIdx) >= nodes.size()) {
            WARNING("glTF node index {} out of range, skipping", nodeIdx);
            continue;
        }

        const auto& node = nodes[static_cast<size_t>(nodeIdx)];

        if (!node.has("mesh")) continue;

        int meshIdx = node["mesh"].getInt();
        if (meshIdx < 0 || static_cast<size_t>(meshIdx) >= meshes.size()) {
            WARNING("glTF mesh index {} out of range, skipping", meshIdx);
            continue;
        }

        parseMesh(meshes[static_cast<size_t>(meshIdx)], pMesh);
    }

    DEBUG(
        "glTF Loaded: {}\n"
        "  Positions : {}\n"
        "  Normals   : {}\n"
        "  Texcoords : {}\n"
        "  Vertices  : {}\n"
        "  Indices   : {}",
        path,
        m_currMesh->positions().size(),
        m_currMesh->normals().size(),
        m_currMesh->texcoords().size(),
        m_currMesh->vertices().size(),
        m_currMesh->indices().size()
    );

    m_root = nullptr;
}

// =========================================================================
// loadBuffers — resolve each buffer entry to raw bytes
// =========================================================================
void GltfParser::loadBuffers(const std::string& gltfDir,
                              const gltf_json::JsonValue& root) {
    if (!root.has("buffers")) return;

    const auto& buffers = root["buffers"];
    m_buffers.resize(buffers.size());

    for (size_t i = 0; i < buffers.size(); ++i) {
        const auto& buf = buffers[i];

        if (!buf.has("byteLength")) {
            WARNING("glTF buffer[{}] missing byteLength, skipping", i);
            continue;
        }

        u64 byteLength = static_cast<u64>(buf["byteLength"].getInt());

        if (buf.has("uri")) {
            const std::string& uri = buf["uri"].getString();

            // ---- embedded base64 data URI -----------------------------------
            if (uri.find("data:") == 0) {
                // format: "data:application/octet-stream;base64,<data>"
                //         or "data:application/gltf-buffer;base64,<data>"
                size_t commaPos = uri.find(',');
                if (commaPos == std::string::npos) {
                    ERROR("Malformed data URI in buffer[{}]", i);
                    continue;
                }
                std::string b64 = uri.substr(commaPos + 1);
                m_buffers[i] = decodeBase64(b64);
            }
            // ---- external .bin file -----------------------------------------
            else {
                std::string binPath = gltfDir + "/" + uri;
                std::ifstream binFile(binPath, std::ios::in | std::ios::binary);
                if (!binFile.is_open()) {
                    ERROR("Failed to open external buffer: {}", binPath.c_str());
                    continue;
                }
                m_buffers[i].resize(static_cast<size_t>(byteLength));
                binFile.read(reinterpret_cast<char*>(m_buffers[i].data()),
                             static_cast<std::streamsize>(byteLength));
                binFile.close();
            }
        } else {
            // Buffer without URI — used in GLB where data comes from BIN chunk.
            // For pure glTF this is unusual; allocate empty space.
            m_buffers[i].resize(static_cast<size_t>(byteLength), 0);
        }

        if (m_buffers[i].size() < byteLength) {
            WARNING("glTF buffer[{}]: expected {} bytes, got {}",
                    i, byteLength, m_buffers[i].size());
        }
    }
}

// =========================================================================
// parseMesh — iterate primitives
// =========================================================================
void GltfParser::parseMesh(const gltf_json::JsonValue& meshJson, Mesh* pMesh) {
    if (!meshJson.has("primitives")) return;

    const auto& primitives = meshJson["primitives"];

    for (size_t pi = 0; pi < primitives.size(); ++pi) {
        parsePrimitive(primitives[pi], pMesh);
    }
}

// =========================================================================
// parsePrimitive — extract vertex attributes + indices from one primitive
// =========================================================================
void GltfParser::parsePrimitive(const gltf_json::JsonValue& primitive,
                                 Mesh* pMesh) {
    if (!m_root || !m_root->has("accessors")) {
        ERROR("No accessors in glTF root");
        return;
    }

    const auto& accessors = (*m_root)["accessors"];

    // ---- read indices (optional — glTF allows non-indexed geometry) --------
    std::vector<u32> primIndices;
    u32               indexCount = 0;

    if (primitive.has("indices")) {
        int idxAccessor = primitive["indices"].getInt();
        if (idxAccessor >= 0 && static_cast<size_t>(idxAccessor) < accessors.size()) {
            const auto& acc = accessors[static_cast<size_t>(idxAccessor)];
            auto raw = readAccessorData(acc);
            u32 compSize = getComponentSize(static_cast<u32>(acc["componentType"].getInt()));

            indexCount = static_cast<u32>(acc["count"].getInt());
            primIndices.reserve(indexCount);

            const u8* data = raw.data();
            for (u32 i = 0; i < indexCount; ++i) {
                u32 idx = 0;
                if (compSize == 1)      idx = data[i];
                else if (compSize == 2) idx = reinterpret_cast<const u16*>(data)[i];
                else if (compSize == 4) idx = reinterpret_cast<const u32*>(data)[i];
                primIndices.push_back(idx);
            }
        }
    }

    // ---- read attributes ----------------------------------------------------
    if (!primitive.has("attributes")) {
        ERROR("glTF primitive has no attributes");
        return;
    }

    const auto& attrs = primitive["attributes"];

    // --- POSITION (required) ---
    if (!attrs.has("POSITION")) {
        ERROR("glTF primitive missing POSITION attribute — required by spec");
        return;
    }

    int posAccIdx = attrs["POSITION"].getInt();
    const auto& posAcc = accessors[static_cast<size_t>(posAccIdx)];
    auto rawPos = readAccessorData(posAcc);
    u32 vertexCount = static_cast<u32>(posAcc["count"].getInt());

    // validate POSITION type — must be VEC3 / FLOAT
    if (posAcc.has("type") && posAcc["type"].getString() != "VEC3") {
        WARNING("POSITION accessor type is {}, expected VEC3", posAcc["type"].getString().c_str());
    }

    const float* posData = reinterpret_cast<const float*>(rawPos.data());

    // --- NORMAL (optional) ---
    std::vector<u8> rawNormal;
    const float* normalData = nullptr;
    if (attrs.has("NORMAL")) {
        int nrmAccIdx = attrs["NORMAL"].getInt();
        rawNormal = readAccessorData(accessors[static_cast<size_t>(nrmAccIdx)]);
        normalData = reinterpret_cast<const float*>(rawNormal.data());
    }

    // --- TEXCOORD_0 (optional) ---
    std::vector<u8> rawTexcoord;
    const float* texcoordData = nullptr;
    if (attrs.has("TEXCOORD_0")) {
        int tcAccIdx = attrs["TEXCOORD_0"].getInt();
        rawTexcoord = readAccessorData(accessors[static_cast<size_t>(tcAccIdx)]);
        texcoordData = reinterpret_cast<const float*>(rawTexcoord.data());
    }

    // ---- build position / normal / texcoord raw arrays ----------------------
    u32 baseVertex = static_cast<u32>(pMesh->vertices().size());

    for (u32 i = 0; i < vertexCount; ++i) {
        pMesh->positions().push_back(glm::vec3(
            posData[i * 3 + 0],
            posData[i * 3 + 1],
            posData[i * 3 + 2]
        ));

        if (normalData) {
            pMesh->normals().push_back(glm::vec3(
                normalData[i * 3 + 0],
                normalData[i * 3 + 1],
                normalData[i * 3 + 2]
            ));
        }

        if (texcoordData) {
            pMesh->texcoords().push_back(glm::vec2(
                texcoordData[i * 2 + 0],
                texcoordData[i * 2 + 1]
            ));
        }
    }

    // ---- build interleaved Vertex array -------------------------------------
    for (u32 i = 0; i < vertexCount; ++i) {
        Vertex v;
        v.pos = glm::vec3(posData[i * 3 + 0], posData[i * 3 + 1], posData[i * 3 + 2]);

        if (normalData)
            v.normal = glm::vec3(normalData[i * 3 + 0], normalData[i * 3 + 1], normalData[i * 3 + 2]);
        else
            v.normal = glm::vec3(0.0f, 0.0f, 1.0f);

        if (texcoordData)
            v.uv = glm::vec2(texcoordData[i * 2 + 0], texcoordData[i * 2 + 1]);
        else
            v.uv = glm::vec2(0.0f, 0.0f);

        // default white color so shader lighting is visible
        v.color = glm::vec3(1.0f, 1.0f, 1.0f);

        pMesh->vertices().push_back(v);
    }

    // ---- build indices (offset by base vertex for multi-primitive meshes) ---
    if (!primIndices.empty()) {
        for (u32 idx : primIndices) {
            pMesh->indices().push_back(baseVertex + idx);
        }
    } else {
        // non-indexed geometry — emit sequential indices
        for (u32 i = 0; i < vertexCount; ++i) {
            pMesh->indices().push_back(baseVertex + i);
        }
    }
}

// =========================================================================
// readAccessorData — resolve accessor → bufferView → buffer → raw bytes
// =========================================================================
std::vector<u8> GltfParser::readAccessorData(const gltf_json::JsonValue& accessor) {
    if (!m_root || !m_root->has("bufferViews")) {
        ERROR("glTF root missing bufferViews");
        return {};
    }

    const auto& bufferViews = (*m_root)["bufferViews"];

    if (!accessor.has("bufferView") || !accessor.has("componentType") || !accessor.has("count")) {
        ERROR("glTF accessor missing required fields");
        return {};
    }

    int bvIndex = accessor["bufferView"].getInt();
    if (bvIndex < 0 || static_cast<size_t>(bvIndex) >= bufferViews.size()) {
        ERROR("glTF accessor bufferView index {} out of range", bvIndex);
        return {};
    }

    const auto& bv = bufferViews[static_cast<size_t>(bvIndex)];

    if (!bv.has("buffer") || !bv.has("byteLength")) {
        ERROR("glTF bufferView missing required fields");
        return {};
    }

    u32 bufferIdx   = static_cast<u32>(bv["buffer"].getInt());
    u32 bvByteOffset = 0;
    u32 bvByteLength = static_cast<u32>(bv["byteLength"].getInt());
    if (bv.has("byteOffset")) {
        bvByteOffset = static_cast<u32>(bv["byteOffset"].getInt());
    }

    if (bufferIdx >= m_buffers.size()) {
        ERROR("glTF bufferView references buffer[{}] but only {} buffers loaded",
              bufferIdx, m_buffers.size());
        return {};
    }

    const auto& buffer = m_buffers[bufferIdx];

    u32 accessorByteOffset = 0;
    if (accessor.has("byteOffset")) {
        accessorByteOffset = static_cast<u32>(accessor["byteOffset"].getInt());
    }

    u32 totalOffset = bvByteOffset + accessorByteOffset;

    if (totalOffset + bvByteLength > buffer.size()) {
        ERROR("glTF accessor data out of bounds: offset={} length={} bufferSize={}",
              totalOffset, bvByteLength, buffer.size());
        return {};
    }

    // Determine actual data size: count × componentSize × typeCount
    u32 componentType = static_cast<u32>(accessor["componentType"].getInt());
    u32 count         = static_cast<u32>(accessor["count"].getInt());
    u32 compSize      = getComponentSize(componentType);
    u32 typeCount     = 1;
    if (accessor.has("type")) {
        typeCount = getTypeCount(accessor["type"].getString());
    }

    u32 dataSize = count * compSize * typeCount;
    if (dataSize > bvByteLength) {
        WARNING("accessor data size {} exceeds bufferView length {} — clamping",
                dataSize, bvByteLength);
        dataSize = bvByteLength;
    }

    std::vector<u8> out(dataSize);
    std::memcpy(out.data(), buffer.data() + totalOffset, dataSize);
    return out;
}
