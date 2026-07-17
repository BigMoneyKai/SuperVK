#pragma once

#include "defines.h"
#include "asset/loader/loader.h"
#include "asset/assets/skeleton_asset.h"

class SkeletonLoader : public AssetLoader {
public:
    b32 load(const String& path, SkeletonAsset& out) {
        out.type  = AssetType::Skeleton;
        out.state = AssetState::Uploaded;
        out.path  = path;
        return true;
    }
};
