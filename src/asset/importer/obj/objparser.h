#pragma once

#include "defines.h"
#include "scene/mesh.h"
#include <fstream>
#include <sstream>

enum FaceType {
    FT_UNKNOWN_TYPE,
    FT_V,
    FT_VT,
    FT_VN,
    FT_VTN,

    FT_MAX_NUM
};

struct FaceIndex {
    u32 vi;
    u32 ti;
    u32 ni;
};

class ObjParser {
public:
    void init();
    void parse(const char* path, Mesh* pMesh);
    void destroy();

private:
    void parse_mtllib(std::stringstream& ss);
    void parse_o(std::stringstream& ss);
    void parse_v(std::stringstream& ss);
    void parse_vn(std::stringstream& ss);
    void parse_vt(std::stringstream& ss);
    void parse_s(std::stringstream& ss);
    void parse_usemtl(std::stringstream& ss);
    void parse_f(std::stringstream& ss);
    
private:
    char* m_objPath;
    std::fstream m_objFile;
    Mesh* m_currMesh;

};
