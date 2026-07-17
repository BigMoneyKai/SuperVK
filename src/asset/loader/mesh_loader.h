#pragma once

#include "defines.h"
#include "asset/loader/loader.h"
#include "asset/assets/mesh_asset.h"
#include "asset/importer/importer_manager.h"

class MeshLoader : public AssetLoader {
public:
    b32 load(ImporterMan& importer, const String& path, MeshAsset& out) {
        out.type  = AssetType::Mesh;
        out.state = AssetState::Uploaded;
        out.path  = path;
        importer.parse(path.c_str(), &out.mesh);
        return b32(!out.mesh.vertices().empty());
    }
};
