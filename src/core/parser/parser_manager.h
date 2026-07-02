#pragma once

#include "core/parser/obj/objparser.h"
#include "core/parser/gltf/gltfparser.h"
#include "core/parser/gltf/glbparser.h"

class ParserMan {
public:
    void init();
    void parse(const char* path, Mesh* mesh);
    void destroy();

private:
    ObjParser m_objParser;
    GltfParser m_gltfParser;
    GlbParser m_glbParser;
};
