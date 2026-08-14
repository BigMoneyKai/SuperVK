#pragma once

#include "asset/assets/mesh_asset.h"
#include "asset/importer/importer_manager.h"
#include "asset/loader/loader.h"

class MeshLoader : public AssetLoader {
public:
  b32 load(ImporterMan &importer, const String &path, MeshAsset &out) {
    out.type = AssetType::mesh;
    out.state = AssetState::uploaded;
    out.path = path;
    importer.parse(path.c_str(), &out.mesh);
    return b32(!out.mesh.vertices().empty());
  }
};
