#pragma once

#include "asset/asset_state.h"
#include "asset/asset_type.h"
#include "asset/assets/animation_asset.h"
#include "asset/handle/asset_handle.h"
#include "core/container/array/array.hpp"
#include "core/container/string/string.hpp"

class AnimationManager {
public:
  using Handle = u64;
  static constexpr Handle INVALID_HANDLE = ~0ull;

  Handle load(const String &path);
  void destroy();

  b32 isValid(Handle handle) const;
  AnimationAsset &get(Handle handle);
  const AnimationAsset &get(Handle handle) const;

private:
  Array<AnimationAsset> m_assets;
};
