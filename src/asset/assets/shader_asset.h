#pragma once

#include "defines.h"
#include "asset/assets/asset.h"
#include "core/container/array/array.hpp"

struct ShaderAsset : Asset {
    ShaderAsset() : Asset{} { type = AssetType::Shader; }

    Array<u32> spirv;
};
