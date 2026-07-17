#pragma once

#include "defines.h"

enum class AssetState : u8 {
    // in static resource
    Unloaded,

    // cpu operation
    Uploading,
    Uploaded,

    // gpu operation
    Loading,
    Loaded,

    Unloading,
    Failed
    
};
