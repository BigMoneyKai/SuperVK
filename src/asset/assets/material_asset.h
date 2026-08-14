#pragma once

#include "asset/assets/asset.h"
#include <glm/glm.hpp>

struct MaterialAsset : Asset {
  MaterialAsset() : Asset{} { type = AssetType::material; }

  glm::vec4 baseColorFactor{1.0f};
  f32 metallicFactor{1.0f};
  f32 roughnessFactor{1.0f};
  glm::vec3 emissiveFactor{0.0f};
  i32 baseColorTextureIndex{-1};
  i32 metallicRoughnessTextureIndex{-1};
  i32 normalTextureIndex{-1};
};
