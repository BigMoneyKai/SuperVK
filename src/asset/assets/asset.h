#pragma once

#include "asset/asset_state.h"
#include "asset/asset_type.h"
#include "core/container/string/string.hpp"
#include "core/define/compiler.h"
#include "core/define/types.h"

struct Asset {
  AssetType type{AssetType::mesh};
  AssetState state{AssetState::unloaded};
  String path;

  Asset() = default;

  SV_FORCE_INLINE b32 isLoaded() const { return state == AssetState::loaded; }
  SV_FORCE_INLINE b32 hasFailed() const { return state == AssetState::failed; }
};
