#pragma once

#include "asset/assets/asset.h"
#include "core/container/array/array.hpp"
#include "core/container/string/string.hpp"
#include <glm/glm.hpp>

struct SkeletonAsset : Asset {
  SkeletonAsset() : Asset{} { type = AssetType::skeleton; }

  Array<String> boneNames;
  Array<glm::mat4> inverseBindMatrices;
};
