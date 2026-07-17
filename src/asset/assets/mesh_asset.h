#pragma once

#include "defines.h"
#include "asset/assets/asset.h"
#include "scene/mesh.h"

struct MeshAsset : Asset {
    MeshAsset() : Asset{} { type = AssetType::Mesh; }

    mesh::Mesh mesh;
};
