#pragma once

#include "asset/asset_type.h"
#include "asset/handle/asset_handle.h"
#include "core/container/string/string.hpp"
#include "core/define/vertex.h"

#include <glm/glm.hpp>

template <typename T> struct hash_key;

template <> struct hash_key<u8>;
template <> struct hash_key<u16>;
template <> struct hash_key<u32>;
template <> struct hash_key<u64>;
template <> struct hash_key<i8>;
template <> struct hash_key<i16>;
template <> struct hash_key<i32>;
template <> struct hash_key<i64>;

template <> struct hash_key<f32>;
template <> struct hash_key<f64>;

template <u64 N> struct hash_key<const char[N]>;
template <> struct hash_key<const char *>;
template <> struct hash_key<String>;

template <typename T> struct hash_key<T *> {
  u64 operator()(const T *key) const;
};

template <> struct hash_key<glm::vec1>;
template <> struct hash_key<glm::vec2>;
template <> struct hash_key<glm::vec3>;
template <> struct hash_key<glm::vec4>;

template <typename T>
  requires std::is_enum_v<T>
struct hash_key<T>;

template <> struct hash_key<Vertex>;

// AssetHandle
template <AssetType T> struct hash_key<AssetHandle<T>>;

#include "hash_key.inl"
