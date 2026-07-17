#pragma once

#include "defines.h"
#include "asset/assets/asset.h"
#include "core/container/string/string.hpp"
#include "core/container/array/array.hpp"
#include <glm/glm.hpp>

struct AnimationChannel {
    i32 nodeIndex{-1};
    String path;
    Array<f32> times;
    Array<f32> values;
};

struct AnimationAsset : Asset {
    AnimationAsset() : Asset{} { type = AssetType::Animation; }

    String name;
    f32 duration{0.0f};
    Array<AnimationChannel> channels;
};
