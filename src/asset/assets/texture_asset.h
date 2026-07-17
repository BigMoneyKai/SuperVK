#pragma once

#include "defines.h"
#include "asset/assets/asset.h"
#include "core/container/array/array.hpp"

struct TextureAsset : Asset {
    TextureAsset() : Asset{} { type = AssetType::Texture; }

    u32 width{0};
    u32 height{0};
    u8  channels{0};
    Array<u8> pixels;
};
