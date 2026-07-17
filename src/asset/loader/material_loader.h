#pragma once

#include "defines.h"
#include "asset/loader/loader.h"
#include "asset/assets/material_asset.h"

class MaterialLoader : public AssetLoader {
public:
    b32 load(const String& path, MaterialAsset& out) {
        out.type  = AssetType::Material;
        out.state = AssetState::Uploaded;
        out.path  = path;
        return true;
    }
};
