#include "asset/importer/obj/objparser.h"
#include "core/debug/debugger.h"
#include <algorithm>
#include <cstdio>

static FaceType scanFaceToken(const std::string& token, FaceIndex* f) {
    u64 slashCount = 0;
    slashCount = std::count(token.begin(), token.end(), '/');

    if(slashCount > 2) {
        ERROR(LogCatag::asset, "Slash count error");
        ERROR(LogCatag::asset, "Unknown obj face format");
        return FaceType::unknown;
    }
    switch(slashCount) {
    // v
    case 0:
    {
        i32 ret = std::sscanf(token.c_str(), "%u", &f->vi);
        if (ret != 1) {
            ERROR(LogCatag::asset, "Failed to parse face position index");
            return FaceType::unknown;
        }
        return FaceType::position;
    }


        // v/t
        case 1:
        {
            i32 ret = std::sscanf(token.c_str(), "%u/%u", &f->vi, &f->ti);
            if (ret != 2) {
                ERROR(LogCatag::asset, "Failed to parse face position/texcoord indices");
                return FaceType::unknown;
            }
            return FaceType::positionTexcoord;
        }

        // v//n and v/t/n
        case 2:
        {
            // v//n
            if (token.find("//") != std::string::npos) {
                i32 ret = std::sscanf(token.c_str(), "%u//%u", &f->vi, &f->ni);
                if (ret != 2) {
                    ERROR(LogCatag::asset, "Failed to parse face position//normal indices");
                    return FaceType::unknown;
                }
                return FaceType::positionNormal;
            }
            // v/t/n
            else {
                i32 ret = std::sscanf(token.c_str(), "%u/%u/%u", &f->vi, &f->ti, &f->ni);
                if (ret != 3) {
                    ERROR(LogCatag::asset, "Failed to parse face position/texcoord/normal indices");
                    return FaceType::unknown;
                }
                return FaceType::positionTexcoordNormal;
            }
        }
        default:
            ERROR(LogCatag::asset, "Unknown obj face format");
        return FaceType::unknown;
    }
    return FaceType::unknown;
}

void ObjParser::init() {
    m_objPath = nullptr;
    m_currMesh = nullptr;
}

void ObjParser::parse(const char* path, Mesh* pMesh) {
    m_objFile.open(path);
    if(!m_objFile.is_open()) {
        ERROR(LogCatag::asset, "Failed to load obj file, go check the path");
        return;
    } m_currMesh = pMesh;

    m_currMesh->positions().clear();
    m_currMesh->texcoords().clear();
    m_currMesh->normals().clear();
    m_currMesh->vertices().clear();
    m_currMesh->indices().clear();
    DEBUG(LogCatag::asset, "Cleared all previous data");

    std::string line;
    while(std::getline(m_objFile, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if(prefix == "mtllib") {
            parse_mtllib(ss);
        }
        else if(prefix == "o") {
            parse_o(ss);
        }
        else if(prefix == "v") {
            parse_v(ss);
        }
        else if(prefix == "vt") {
            parse_vt(ss);
        }
        else if(prefix == "vn") {
            parse_vn(ss);
        }
        else if(prefix == "usemtl") {
            parse_usemtl(ss);
        }
        else if(prefix == "s") {
            parse_s(ss);
        }
        else if(prefix == "f") {
            parse_f(ss);
        }
    }
    DEBUG(LogCatag::asset,
        "OBJ Loaded\n"
        "Positions : {}\n"
        "Normals   : {}\n"
        "Texcoords : {}\n"
        "Vertices  : {}\n"
        "Indices   : {}",
        m_currMesh->positions().size(),
        m_currMesh->normals().size(),
        m_currMesh->texcoords().size(),
        m_currMesh->vertices().size(),
        m_currMesh->indices().size()
    );
}

void ObjParser::destroy() {
    if(m_objFile.is_open()) {
        m_objFile.close();
    }
    m_objPath = nullptr;
    m_currMesh = nullptr;
}

void ObjParser::parse_mtllib(std::stringstream& ss) {
    std::string lib;
    ss >> lib;
}

void ObjParser::parse_o(std::stringstream& ss) {
    std::string name;
    ss >> name;
}

void ObjParser::parse_v(std::stringstream& ss) {
    glm::vec3 vertex;
    ss >> vertex.x >> vertex.y >> vertex.z;
    m_currMesh->positions().push_back(vertex);
}

void ObjParser::parse_vn(std::stringstream& ss) {
    glm::vec3 normal;
    ss >> normal.x >> normal.y >> normal.z;
    m_currMesh->normals().push_back(normal);
}

void ObjParser::parse_vt(std::stringstream& ss) {
    glm::vec2 texcoord;
    ss >> texcoord.x >> texcoord.y;
    m_currMesh->texcoords().push_back(texcoord);
}

void ObjParser::parse_s(std::stringstream& ss) {
    std::string s;
    ss >> s;
}

void ObjParser::parse_usemtl(std::stringstream& ss) {
    std::string usemtl;
    ss >> usemtl;
}

void ObjParser::parse_f(std::stringstream& ss) {
    std::string token;

    std::vector<FaceIndex> faceIndices;
    FaceType fType = FaceType::unknown;
    while(ss >> token) {
        FaceIndex faceIndex;
        fType = scanFaceToken(token, &faceIndex);
        faceIndices.push_back(faceIndex);
    }

    if(faceIndices.size() < 3) {
        ERROR(LogCatag::asset, "Wrong obj file format, the number of face vertex should be 3 at least");
    }

    for(i32 i = 1; i < faceIndices.size() - 1; i++) {
        FaceIndex triangle[3] = {
            faceIndices[0],
            faceIndices[i],
            faceIndices[i + 1],
        };

        for(i32 j = 0; j < 3; j++) {
            Vertex vertex;

            switch(fType) {
                case FaceType::position:
                    vertex.pos = m_currMesh->positions().at(triangle[j].vi - 1);
                    break;
                case FaceType::positionTexcoord:
                    vertex.pos = m_currMesh->positions().at(triangle[j].vi - 1);
                    vertex.uv = m_currMesh->texcoords().at(triangle[j].ti - 1);
                    break;
                case FaceType::positionNormal:
                    vertex.pos = m_currMesh->positions().at(triangle[j].vi - 1);
                    vertex.normal = m_currMesh->normals().at(triangle[j].ni - 1);
                    break;
                case FaceType::positionTexcoordNormal:
                    vertex.pos = m_currMesh->positions().at(triangle[j].vi - 1);
                    vertex.uv = m_currMesh->texcoords().at(triangle[j].ti - 1);
                    vertex.normal = m_currMesh->normals().at(triangle[j].ni - 1);
                    break;
                case FaceType::unknown:
                case FaceType::maxNum:
                default:
                    // Handle unknown types - skip
                    break;
            }

            m_currMesh->vertices().push_back(vertex);
            m_currMesh->indices().push_back(m_currMesh->vertices().size() - 1);
        }
    }
}
