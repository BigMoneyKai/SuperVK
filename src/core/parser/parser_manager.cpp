#include "core/parser/parser_manager.h"
#include "scene/mesh.h"
#include "debug/debug.h"
#include <cstring>

void ParserMan::init() {
    m_objParser.init();
    m_gltfParser.init();
    m_glbParser.init();
}

void ParserMan::parse(const char* path, Mesh* mesh) {
    if(!path || !mesh) return;

    // Determine file type from extension
    const char* ext = strrchr(path, '.');
    if(!ext) {
        ERROR("Failed to determine file type");
        return;
    }

    if(strcmp(ext, ".obj") == 0) {
        m_objParser.parse(path, mesh);
        DEBUG("Loaded OBJ file: %s", path);
    } else if(strcmp(ext, ".gltf") == 0) {
        m_gltfParser.parse(path, mesh);
        DEBUG("Loaded GLTF file: %s", path);
    } else if(strcmp(ext, ".glb") == 0) {
        m_glbParser.parse(path, mesh);
        DEBUG("Loaded GLB file: %s", path);
    } else {
        ERROR("Unsupported file format: %s", ext);
    }
}

void ParserMan::destroy() {
    m_objParser.destroy();
    m_gltfParser.destroy();
    m_glbParser.destroy();
}
