#pragma once

#include "defines.h"
#include <glm/glm.hpp>
#include "core/container/string/string.hpp"

template<typename T>
struct hash_key;

template<> struct hash_key<u8>;
template<> struct hash_key<u16>;
template<> struct hash_key<u32>;
template<> struct hash_key<u64>;
template<> struct hash_key<i8>;
template<> struct hash_key<i16>;
template<> struct hash_key<i32>;
template<> struct hash_key<i64>;

template<> struct hash_key<f32>;
template<> struct hash_key<f64>;

template<u64 N> struct hash_key<const char[N]>;
template<> struct hash_key<String>;

template<typename T>
struct hash_key<T*> {
    u64 operator()(const T* key) const;
};

template<> struct hash_key<glm::vec1>;
template<> struct hash_key<glm::vec2>;
template<> struct hash_key<glm::vec3>;
template<> struct hash_key<glm::vec4>;

#include "hash_key.inl"
