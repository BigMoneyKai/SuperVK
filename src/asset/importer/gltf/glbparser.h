#pragma once

#include "scene/mesh.h"

#include "json_parser.hpp"

#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// GlbParser — GLB binary container (glTF 2.0 Binary) file parser
//
// GLB layout:
//   Header  (12 bytes):  magic(u32) | version(u32) | fileLength(u32)
//   Chunk 0 (JSON):      chunkLength(u32) | chunkType(u32=0x4E4F534A) | data
//   Chunk 1 (BIN):       chunkLength(u32) | chunkType(u32=0x004E4942) | data
//
// The JSON chunk is parsed identically to a .gltf file; buffers without
// a "uri" field reference the BIN chunk data (buffer index 0).
// ---------------------------------------------------------------------------
class GlbParser {
public:
  void init();
  void parse(const char *path, Mesh *pMesh);
  void destroy();

private:
  // ---- GLB binary header constants ----------------------------------------
  static constexpr u32 kGlbMagic = 0x46546C67;  // "glTF" little-endian
  static constexpr u32 kChunkJson = 0x4E4F534A; // "JSON"
  static constexpr u32 kChunkBin = 0x004E4942;  // "BIN\0"

  // ---- mesh / primitive traversal (same logic as glTF) --------------------
  void parseMesh(const gltf_json::JsonValue &meshJson, Mesh *pMesh);
  void parsePrimitive(const gltf_json::JsonValue &primitive, Mesh *pMesh);

  // ---- accessor reading ---------------------------------------------------
  std::vector<u8> readAccessorData(const gltf_json::JsonValue &accessor);
  u32 getComponentSize(u32 componentType) const;
  u32 getTypeCount(const std::string &type) const;

private:
  // Per-file state — reset on each parse() call
  const gltf_json::JsonValue *m_root = nullptr;
  std::vector<std::vector<u8>> m_buffers; // raw binary data per buffer index
  Mesh *m_currMesh = nullptr;
};
