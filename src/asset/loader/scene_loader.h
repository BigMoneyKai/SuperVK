#pragma once

#include "asset/assets/scene_asset.h"
#include "asset/loader/loader.h"

class SceneLoader : public AssetLoader {
public:
  b32 load(const String &path, SceneAsset &out) {
    out.type = AssetType::scene;
    out.state = AssetState::uploaded;
    out.path = path;
    return true;
  }
};
