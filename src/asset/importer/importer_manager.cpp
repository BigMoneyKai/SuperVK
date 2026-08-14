#include "asset/importer/importer_manager.h"
#include "scene/mesh.h"
#include "core/debug/debugger.h"
#include <cstring>

void ImporterMan::init() {
    m_objParser.init();
    m_gltfParser.init();
    m_glbParser.init();
}

void ImporterMan::parse(const char* path, Mesh* mesh) {
    if(!path || !mesh) return;

    // Determine file type from extension
    const char* ext = strrchr(path, '.');
    if(!ext) {
        ERROR(LogCatag::asset, "Failed to determine file type");
        return;
    }

    if(strcmp(ext, ".obj") == 0) {
        m_objParser.parse(path, mesh);
        DEBUG(LogCatag::asset, "Loaded OBJ file: {}", path);
    } else if(strcmp(ext, ".gltf") == 0) {
        m_gltfParser.parse(path, mesh);
        DEBUG(LogCatag::asset, "Loaded GLTF file: {}", path);
    } else if(strcmp(ext, ".glb") == 0) {
        m_glbParser.parse(path, mesh);
        DEBUG(LogCatag::asset, "Loaded GLB file: {}", path);
    } else {
        ERROR(LogCatag::asset, "Unsupported file format: {}", ext);
    }
}

void ImporterMan::destroy() {
    m_objParser.destroy();
    m_gltfParser.destroy();
    m_glbParser.destroy();
}
