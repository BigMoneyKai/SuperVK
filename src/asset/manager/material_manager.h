#pragma once

#include "defines.h"
#include "asset/handle/asset_handle.h"
#include "asset/asset_type.h"
#include "asset/asset_state.h"
#include "asset/assets/material_asset.h"
#include "core/container/array/array.hpp"
#include "core/container/string/string.hpp"

class MaterialManager {
public:
    using Handle = u64;
    static constexpr Handle INVALID_HANDLE = ~0ull;

    Handle load(const String& path);
    void destroy();

    b32 isValid(Handle handle) const;
    MaterialAsset& get(Handle handle);
    const MaterialAsset& get(Handle handle) const;

private:
    Array<MaterialAsset> m_assets;
};
