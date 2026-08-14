#pragma once

#include "asset/assets/asset.h"
#include "scene/mesh.h"

struct MeshAsset : Asset {
  MeshAsset() : Asset{} { type = AssetType::mesh; }

  mesh::Mesh mesh;
};
