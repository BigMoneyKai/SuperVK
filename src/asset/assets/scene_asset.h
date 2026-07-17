#pragma once

#include "defines.h"
#include "asset/assets/asset.h"
#include "core/container/string/string.hpp"

struct SceneAsset : Asset {
    SceneAsset() : Asset{} { type = AssetType::Scene; }

    String name;
};
