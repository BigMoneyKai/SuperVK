#pragma once

#include "defines.h"
#include "asset/loader/loader.h"
#include "asset/assets/scene_asset.h"

class SceneLoader : public AssetLoader {
public:
    b32 load(const String& path, SceneAsset& out) {
        out.type  = AssetType::Scene;
        out.state = AssetState::Uploaded;
        out.path  = path;
        return true;
    }
};
