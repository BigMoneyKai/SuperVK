#pragma once

#include "asset_data.h"
#include "core/container/array/array.hpp"
#include "core/container/string/string.hpp"
#include "core/define/vertex.h"
#include "core/memory/allocator.h"

#include <glm/glm.hpp>

struct AnimationChannel {
  i32 nodeIndex{-1};
  String path{&default_allocator()};
  Array<f32> times;
  Array<f32> values;
};

template <> struct AssetData<AssetType::animation> {
  String name{&default_allocator()};
  f32 duration{0.0f};
  Array<AnimationChannel> channels;
};

template <> struct AssetData<AssetType::mesh> {
  Array<Vertex> vertices;
  Array<u32> indices;
};

template <> struct AssetData<AssetType::shader> {
  String vertSrc;
  String fragSrc;
};

template <> struct AssetData<AssetType::scene> {
  String name;

  // TODO: Add node cascade
};

template <> struct AssetData<AssetType::material> {
  glm::vec4 baseColorFactor{1.0f};
  f32 metallicFactor{1.0f};
  f32 roughnessFactor{1.0f};
  glm::vec3 emissiveFactor{0.0f};
  i32 baseColorTextureIndex{-1};
  i32 metallicRoughnessTextureIndex{-1};
  i32 normalTextureIndex{-1};
};

template <> struct AssetData<AssetType::skeleton> {
  Array<String> boneNames{&default_allocator()};
  Array<glm::mat4> inverseBindMatrices;
};

template <> struct AssetData<AssetType::texture> {
  u32 width = 0;
  u32 height = 0;
  u8 channels = 0;
  Array<u8> pixels; // raw pixel data
};
