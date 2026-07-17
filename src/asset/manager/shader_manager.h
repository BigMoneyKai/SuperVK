#pragma once

#include "defines.h"
#include "asset/handle/asset_handle.h"
#include "asset/asset_type.h"
#include "asset/asset_state.h"
#include "asset/assets/shader_asset.h"
#include "core/container/array/array.hpp"
#include "core/container/string/string.hpp"
#include "core/job/job_system.h"
#include <cstring>

class ShaderManager {
public:
    using Handle = u64;
    static constexpr Handle INVALID_HANDLE = ~0ull;

    void init(JobSystem* js);
    Handle load(const String& path);
    void destroy();

    b32 isValid(Handle handle) const;
    ShaderAsset& get(Handle handle);
    const ShaderAsset& get(Handle handle) const;

    void update();

private:
    struct LoadJobData {
        ShaderManager* self;
        char path[256];
        Handle handle;
    };
    static void jobFunc(void* data);

    JobSystem* m_jobSystem{nullptr};
    Array<ShaderAsset> m_assets;
};
