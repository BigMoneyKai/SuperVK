#include "core/parser/gltf/glbparser.h"

#include "debug/debugger.h"

#include <fstream>
#include <cstring>
#include <glm/glm.hpp>

// =========================================================================
// Helpers
// =========================================================================
u32 GlbParser::getComponentSize(u32 componentType) const {
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

u32 GlbParser::getTypeCount(const std::string& type) const {
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
void GlbParser::init() {
    m_root     = nullptr;
    m_currMesh = nullptr;
}

void GlbParser::destroy() {
    m_buffers.clear();
    m_root     = nullptr;
    m_currMesh = nullptr;
}

// =========================================================================
// Main entry point — parse GLB binary container
// =========================================================================
void GlbParser::parse(const char* path, Mesh* pMesh) {
    if (!path || !pMesh) {
        ERROR("GlbParser::parse — null path or mesh pointer");
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

    // ---- read entire .glb file as binary -----------------------------------
    std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        ERROR("Failed to open GLB file: {}", path);
        return;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> rawFile(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(rawFile.data()), fileSize)) {
        ERROR("Failed to read GLB file: {}", path);
        return;
    }
    file.close();

    // ---- parse header (12 bytes) -------------------------------------------
    if (fileSize < 12) {
        ERROR("GLB file too small for header: {} ({} bytes)", path, static_cast<long>(fileSize));
        return;
    }

    u32 magic      = *reinterpret_cast<u32*>(rawFile.data() + 0);
    u32 version    = *reinterpret_cast<u32*>(rawFile.data() + 4);
    u32 totalLength = *reinterpret_cast<u32*>(rawFile.data() + 8);

    if (magic != kGlbMagic) {
        ERROR("GLB invalid magic: 0x%08X (expected 0x%08X)", magic, kGlbMagic);
        return;
    }

    if (version != 2) {
        WARNING("GLB version {} — parser targets version 2", version);
    }

    if (static_cast<u64>(totalLength) != static_cast<u64>(fileSize)) {
        WARNING("GLB header length {} != file size {}", totalLength, static_cast<long>(fileSize));
    }

    // ---- parse chunks -------------------------------------------------------
    std::string jsonStr;
    std::vector<u8> binData;

    size_t offset = 12;

    while (offset + 8 <= rawFile.size()) {
        u32 chunkLength = *reinterpret_cast<u32*>(rawFile.data() + offset);
        u32 chunkType   = *reinterpret_cast<u32*>(rawFile.data() + offset + 4);
        offset += 8;

        if (offset + chunkLength > rawFile.size()) {
            ERROR("GLB chunk length {} exceeds file bounds at offset {}", chunkLength, offset);
            return;
        }

        if (chunkType == kChunkJson) {
            jsonStr.assign(
                reinterpret_cast<const char*>(rawFile.data() + offset),
                chunkLength);
        } else if (chunkType == kChunkBin) {
            binData.assign(
                rawFile.data() + offset,
                rawFile.data() + offset + chunkLength);
        } else {
            WARNING("GLB unknown chunk type: 0x%08X — skipping", chunkType);
        }

        offset += chunkLength;
    }

    if (jsonStr.empty()) {
        ERROR("GLB file contains no JSON chunk: {}", path);
        return;
    }

    // ---- parse JSON ---------------------------------------------------------
    gltf_json::JsonValue root = gltf_json::parse(jsonStr);
    if (!root.isObject()) {
        ERROR("GLB JSON chunk is not a valid JSON object");
        return;
    }
    m_root = &root;

    // ---- validate asset version --------------------------------------------
    if (root.has("asset") && root["asset"].has("version")) {
        const std::string& ver = root["asset"]["version"].getString();
        if (ver[0] != '2') {
            WARNING("glTF version {} — parser targets 2.0", ver.c_str());
        }
    }

    // ---- prepare buffers ----------------------------------------------------
    // In GLB, the BIN chunk provides the data for buffer[0] (and possibly
    // others if there are multiple buffers with no "uri" field).
    if (root.has("buffers")) {
        const auto& buffers = root["buffers"];
        m_buffers.resize(buffers.size());

        for (size_t i = 0; i < buffers.size(); ++i) {
            const auto& buf = buffers[i];

            u64 byteLength = 0;
            if (buf.has("byteLength")) {
                byteLength = static_cast<u64>(buf["byteLength"].getInt());
            }

            if (buf.has("uri")) {
                // Embedded base64 data URI (rare in GLB but spec allows it)
                const std::string& uri = buf["uri"].getString();
                if (uri.find("data:") == 0) {
                    size_t commaPos = uri.find(',');
                    if (commaPos != std::string::npos) {
                        // Simple base64 decode
                        std::string b64 = uri.substr(commaPos + 1);
                        m_buffers[i].reserve((b64.size() * 3) / 4);
                        // Use a simple static decode — we don't have GltfParser's method here
                        // but for GLB the BIN chunk is the primary data source
                    }
                    WARNING("GLB buffer[{}] uses data URI — not fully supported in GLB mode", i);
                } else {
                    ERROR("GLB buffer[{}] references external file '{}' — "
                          "GLB buffers should be embedded or use the BIN chunk",
                          i, uri.c_str());
                }
            } else {
                // Buffer without URI → uses BIN chunk data
                if (i == 0 && !binData.empty()) {
                    if (binData.size() >= byteLength) {
                        m_buffers[i] = std::move(binData);
                    } else {
                        WARNING("GLB BIN chunk size {} < buffer[0].byteLength {}",
                                binData.size(), byteLength);
                        m_buffers[i] = std::move(binData);
                    }
                } else if (i == 0 && binData.empty()) {
                    WARNING("GLB has buffer without URI but no BIN chunk present");
                    m_buffers[i].resize(static_cast<size_t>(byteLength), 0);
                } else {
                    // buffer[1+] without URI — the spec allows all buffers to
                    // share the same BIN chunk with different byteOffsets
                    WARNING("GLB buffer[{}] without URI — multiple-buffer GLB "
                            "not fully supported", i);
                    m_buffers[i].resize(static_cast<size_t>(byteLength), 0);
                }
            }
        }
    }

    // ---- traverse scene → nodes → meshes -----------------------------------
    if (!root.has("scenes") || !root.has("nodes") || !root.has("meshes")) {
        ERROR("GLB missing required arrays (scenes/nodes/meshes)");
        return;
    }

    const auto& scenes = root["scenes"];
    const auto& nodes  = root["nodes"];
    const auto& meshes = root["meshes"];

    int sceneIdx = 0;
    if (root.has("scene")) {
        sceneIdx = root["scene"].getInt();
    }

    if (sceneIdx < 0 || static_cast<size_t>(sceneIdx) >= scenes.size()) {
        ERROR("GLB default scene index {} out of range", sceneIdx);
        return;
    }

    const auto& scene = scenes[static_cast<size_t>(sceneIdx)];
    if (!scene.has("nodes")) {
        ERROR("GLB scene[{}] has no nodes", sceneIdx);
        return;
    }

    const auto& sceneNodes = scene["nodes"];

    for (size_t ni = 0; ni < sceneNodes.size(); ++ni) {
        int nodeIdx = sceneNodes[ni].getInt();
        if (nodeIdx < 0 || static_cast<size_t>(nodeIdx) >= nodes.size()) {
            WARNING("GLB node index {} out of range", nodeIdx);
            continue;
        }

        const auto& node = nodes[static_cast<size_t>(nodeIdx)];
        if (!node.has("mesh")) continue;

        int meshIdx = node["mesh"].getInt();
        if (meshIdx < 0 || static_cast<size_t>(meshIdx) >= meshes.size()) {
            WARNING("GLB mesh index {} out of range", meshIdx);
            continue;
        }

        parseMesh(meshes[static_cast<size_t>(meshIdx)], pMesh);
    }

    DEBUG(
        "GLB Loaded: {}\n"
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
// parseMesh — iterate primitives
// =========================================================================
void GlbParser::parseMesh(const gltf_json::JsonValue& meshJson, Mesh* pMesh) {
    if (!meshJson.has("primitives")) return;

    const auto& primitives = meshJson["primitives"];
    for (size_t pi = 0; pi < primitives.size(); ++pi) {
        parsePrimitive(primitives[pi], pMesh);
    }
}

// =========================================================================
// parsePrimitive — extract vertex attributes + indices
// =========================================================================
void GlbParser::parsePrimitive(const gltf_json::JsonValue& primitive,
                                Mesh* pMesh) {
    if (!m_root || !m_root->has("accessors")) {
        ERROR("No accessors in GLB JSON");
        return;
    }

    const auto& accessors = (*m_root)["accessors"];

    // ---- read indices -------------------------------------------------------
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
        ERROR("GLB primitive has no attributes");
        return;
    }

    const auto& attrs = primitive["attributes"];

    // --- POSITION (required) ---
    if (!attrs.has("POSITION")) {
        ERROR("GLB primitive missing POSITION attribute");
        return;
    }

    int posAccIdx = attrs["POSITION"].getInt();
    const auto& posAcc = accessors[static_cast<size_t>(posAccIdx)];
    auto rawPos = readAccessorData(posAcc);
    u32 vertexCount = static_cast<u32>(posAcc["count"].getInt());

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

    // ---- build raw arrays ---------------------------------------------------
    u32 baseVertex = static_cast<u32>(pMesh->vertices().size());

    for (u32 i = 0; i < vertexCount; ++i) {
        pMesh->positions().push_back(glm::vec3(
            posData[i * 3 + 0], posData[i * 3 + 1], posData[i * 3 + 2]));

        if (normalData) {
            pMesh->normals().push_back(glm::vec3(
                normalData[i * 3 + 0], normalData[i * 3 + 1], normalData[i * 3 + 2]));
        }

        if (texcoordData) {
            pMesh->texcoords().push_back(glm::vec2(
                texcoordData[i * 2 + 0], texcoordData[i * 2 + 1]));
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

        v.color = glm::vec3(1.0f, 1.0f, 1.0f);

        pMesh->vertices().push_back(v);
    }

    // ---- build indices ------------------------------------------------------
    if (!primIndices.empty()) {
        for (u32 idx : primIndices) {
            pMesh->indices().push_back(baseVertex + idx);
        }
    } else {
        for (u32 i = 0; i < vertexCount; ++i) {
            pMesh->indices().push_back(baseVertex + i);
        }
    }
}

// =========================================================================
// readAccessorData — resolve accessor → bufferView → buffer → raw bytes
// =========================================================================
std::vector<u8> GlbParser::readAccessorData(const gltf_json::JsonValue& accessor) {
    if (!m_root || !m_root->has("bufferViews")) {
        ERROR("GLB root missing bufferViews");
        return {};
    }

    const auto& bufferViews = (*m_root)["bufferViews"];

    if (!accessor.has("bufferView") || !accessor.has("componentType") || !accessor.has("count")) {
        ERROR("GLB accessor missing required fields");
        return {};
    }

    int bvIndex = accessor["bufferView"].getInt();
    if (bvIndex < 0 || static_cast<size_t>(bvIndex) >= bufferViews.size()) {
        ERROR("GLB accessor bufferView index {} out of range", bvIndex);
        return {};
    }

    const auto& bv = bufferViews[static_cast<size_t>(bvIndex)];

    if (!bv.has("buffer") || !bv.has("byteLength")) {
        ERROR("GLB bufferView missing required fields");
        return {};
    }

    u32 bufferIdx    = static_cast<u32>(bv["buffer"].getInt());
    u32 bvByteOffset = 0;
    u32 bvByteLength = static_cast<u32>(bv["byteLength"].getInt());
    if (bv.has("byteOffset")) {
        bvByteOffset = static_cast<u32>(bv["byteOffset"].getInt());
    }

    if (bufferIdx >= m_buffers.size()) {
        ERROR("GLB bufferView references buffer[{}] but only {} buffers loaded",
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
        ERROR("GLB accessor data out of bounds: offset={} length={} bufferSize={}",
              totalOffset, bvByteLength, buffer.size());
        return {};
    }

    u32 componentType = static_cast<u32>(accessor["componentType"].getInt());
    u32 count         = static_cast<u32>(accessor["count"].getInt());
    u32 compSize      = getComponentSize(componentType);
    u32 typeCount     = 1;
    if (accessor.has("type")) {
        typeCount = getTypeCount(accessor["type"].getString());
    }

    u32 dataSize = count * compSize * typeCount;
    if (dataSize > bvByteLength) {
        dataSize = bvByteLength;
    }

    std::vector<u8> out(dataSize);
    std::memcpy(out.data(), buffer.data() + totalOffset, dataSize);
    return out;
}
