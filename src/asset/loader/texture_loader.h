#pragma once

#include "defines.h"
#include "asset/loader/loader.h"
#include "asset/assets/texture_asset.h"

#include <stb_image.h>
#include <cstring>

class TextureLoader : public AssetLoader {
public:
    b32 load(const String& path, TextureAsset& out) {
        out.type  = AssetType::Texture;
        out.state = AssetState::Uploaded;
        out.path  = path;

        Array<u8> fileData;
        if (!loadFile(path, fileData))
            return false;

        i32 w = 0, h = 0, c = 0;
        stbi_uc* img = stbi_load_from_memory(
            fileData.data(), static_cast<i32>(fileData.size()),
            &w, &h, &c, 0);
        if (!img) return false;

        out.width    = static_cast<u32>(w);
        out.height   = static_cast<u32>(h);
        out.channels = static_cast<u8>(c);

        u64 pixelCount = static_cast<u64>(w) * h * c;
        out.pixels.resize(pixelCount);
        std::memcpy(out.pixels.data(), img, pixelCount);
        stbi_image_free(img);
        return true;
    }
};
