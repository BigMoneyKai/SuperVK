#pragma once

#include "defines.h"
#include "scene/mesh.h"

#include "json_parser.hpp"

#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// GltfParser — glTF 2.0 (JSON) file parser
//
// Reads .gltf files and populates a Mesh with vertex data extracted from
// accessors / bufferViews / buffers.
//
// Supports:
//   - External .bin buffer files
//   - Embedded base64 data-URI buffers
//   - POSITION, NORMAL, TEXCOORD_0 attributes
//   - Indexed geometry (UNSIGNED_SHORT / UNSIGNED_INT)
//   - Default scene → node → mesh traversal
//   - Multiple primitives per mesh (merged)
// ---------------------------------------------------------------------------
class GltfParser {
public:
    void init();
    void parse(const char* path, Mesh* pMesh);
    void destroy();

private:
    // ---- buffer loading -----------------------------------------------------
    void loadBuffers(const std::string& gltfDir,
                     const gltf_json::JsonValue& root);

    // ---- mesh / primitive traversal -----------------------------------------
    void parseMesh(const gltf_json::JsonValue& meshJson, Mesh* pMesh);
    void parsePrimitive(const gltf_json::JsonValue& primitive, Mesh* pMesh);

    // ---- accessor reading ---------------------------------------------------
    std::vector<u8>  readAccessorData(const gltf_json::JsonValue& accessor);
    u32              getComponentSize(u32 componentType) const;
    u32              getTypeCount(const std::string& type) const;

    // ---- base64 -------------------------------------------------------------
    std::vector<u8> decodeBase64(const std::string& encoded) const;

    // ---- helpers ------------------------------------------------------------
    static std::string dirName(const std::string& path);

private:
    // Per-file state — reset on each parse() call
    const gltf_json::JsonValue* m_root = nullptr;
    std::vector<std::vector<u8>> m_buffers;   // raw binary data per buffer index
    Mesh*                        m_currMesh = nullptr;
};
