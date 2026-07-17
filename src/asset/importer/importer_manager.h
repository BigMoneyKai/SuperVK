#pragma once

#include "asset/importer/obj/objparser.h"
#include "asset/importer/gltf/gltfparser.h"
#include "asset/importer/gltf/glbparser.h"

class ImporterMan {
public:
    void init();
    void parse(const char* path, Mesh* mesh);
    void destroy();

private:
    ObjParser m_objParser;
    GltfParser m_gltfParser;
    GlbParser m_glbParser;
};
