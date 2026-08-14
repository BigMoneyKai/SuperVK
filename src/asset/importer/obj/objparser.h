#pragma once

#include "core/define/types.h"
#include "scene/mesh.h"
#include <fstream>
#include <sstream>

enum class FaceType {
  unknown,
  position,
  positionTexcoord,
  positionNormal,
  positionTexcoordNormal,

  maxNum
};

struct FaceIndex {
  u32 vi;
  u32 ti;
  u32 ni;
};

class ObjParser {
public:
  void init();
  void parse(const char *path, Mesh *pMesh);
  void destroy();

private:
  void parse_mtllib(std::stringstream &ss);
  void parse_o(std::stringstream &ss);
  void parse_v(std::stringstream &ss);
  void parse_vn(std::stringstream &ss);
  void parse_vt(std::stringstream &ss);
  void parse_s(std::stringstream &ss);
  void parse_usemtl(std::stringstream &ss);
  void parse_f(std::stringstream &ss);

private:
  char *m_objPath;
  std::fstream m_objFile;
  Mesh *m_currMesh;
};
