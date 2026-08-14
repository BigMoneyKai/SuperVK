#pragma once

#include "asset/assets/animation_asset.h"
#include "asset/loader/loader.h"
#include "core/define/types.h"

class AnimationLoader : public AssetLoader {
public:
  b32 load(const String &path, AnimationAsset &out) {
    out.type = AssetType::animation;
    out.state = AssetState::uploaded;
    out.path = path;
    return SV_TRUE;
  }
};
