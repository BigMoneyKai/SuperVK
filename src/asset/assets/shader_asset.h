#pragma once

#include "asset/assets/asset.h"
#include "core/container/array/array.hpp"

struct ShaderAsset : Asset {
  ShaderAsset() : Asset{} { type = AssetType::shader; }

  Array<u32> spirv;
};
