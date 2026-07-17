#pragma once

#include "defines.h"
#include "core/container/array/array.hpp"
#include "core/container/string/string.hpp"

#include <span>

class AssetLoader {
public:
    virtual ~AssetLoader() = default;

    b32 loadFile(const String& path, Array<u8>& output);
    b32 loadText(const String& path, String& output);
    b32 saveFile(const String& path, std::span<const u8> data);
};
