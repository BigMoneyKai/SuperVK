#pragma once

#include "defines.h"
#include "asset/loader/loader.h"
#include "asset/assets/animation_asset.h"

class AnimationLoader : public AssetLoader {
public:
    b32 load(const String& path, AnimationAsset& out) {
        out.type  = AssetType::Animation;
        out.state = AssetState::Uploaded;
        out.path  = path;
        return true;
    }
};
