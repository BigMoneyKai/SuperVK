#pragma once

#include "asset/assets/asset.h"
#include "core/container/string/string.hpp"

struct SceneAsset : Asset {
  SceneAsset() : Asset{} { type = AssetType::scene; }

  String name;
};
