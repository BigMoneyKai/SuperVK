#pragma once

#include "defines.h"
#include "asset/assets/asset.h"
#include "core/container/string/string.hpp"
#include "core/container/array/array.hpp"
#include <glm/glm.hpp>

struct SkeletonAsset : Asset {
    SkeletonAsset() : Asset{} { type = AssetType::Skeleton; }

    Array<String> boneNames;
    Array<glm::mat4> inverseBindMatrices;
};
