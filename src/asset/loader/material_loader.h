#pragma once

#include "asset/assets/material_asset.h"
#include "asset/loader/loader.h"

class MaterialLoader : public AssetLoader {
public:
  b32 load(const String &path, MaterialAsset &out) {
    out.type = AssetType::material;
    out.state = AssetState::uploaded;
    out.path = path;
    return true;
  }
};
