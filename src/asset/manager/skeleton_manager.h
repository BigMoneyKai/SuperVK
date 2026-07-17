#pragma once

#include "defines.h"
#include "asset/handle/asset_handle.h"
#include "asset/asset_type.h"
#include "asset/asset_state.h"
#include "asset/assets/skeleton_asset.h"
#include "core/container/array/array.hpp"
#include "core/container/string/string.hpp"

class SkeletonManager {
public:
    using Handle = u64;
    static constexpr Handle INVALID_HANDLE = ~0ull;

    Handle load(const String& path);
    void destroy();

    b32 isValid(Handle handle) const;
    SkeletonAsset& get(Handle handle);
    const SkeletonAsset& get(Handle handle) const;

private:
    Array<SkeletonAsset> m_assets;
};
