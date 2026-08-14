#pragma once

#include "asset/assets/skeleton_asset.h"
#include "asset/loader/loader.h"

class SkeletonLoader : public AssetLoader {
public:
  b32 load(const String &path, SkeletonAsset &out) {
    out.type = AssetType::skeleton;
    out.state = AssetState::uploaded;
    out.path = path;
    return true;
  }
};
