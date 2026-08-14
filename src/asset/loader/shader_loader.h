#pragma once

#include "asset/assets/shader_asset.h"
#include "asset/loader/loader.h"
#include <cstring>

class ShaderLoader : public AssetLoader {
public:
  b32 load(const String &path, ShaderAsset &out) {
    out.type = AssetType::shader;
    out.state = AssetState::uploaded;
    out.path = path;

    Array<u8> fileData;
    if (!loadFile(path, fileData))
      return false;

    u64 wordCount = fileData.size() / sizeof(u32);
    out.spirv.resize(wordCount);
    std::memcpy(out.spirv.data(), fileData.data(), fileData.size());
    return true;
  }
};
