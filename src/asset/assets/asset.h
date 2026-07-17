#pragma once

#include "defines.h"
#include "asset/asset_type.h"
#include "asset/asset_state.h"
#include "core/container/string/string.hpp"

struct Asset {
    AssetType   type{AssetType::Mesh};
    AssetState  state{AssetState::Unloaded};
    String      path;

    Asset() = default;

    SV_FORCE_INLINE b32 isLoaded() const { return state == AssetState::Loaded; }
    SV_FORCE_INLINE b32 hasFailed() const { return state == AssetState::Failed; }
};
