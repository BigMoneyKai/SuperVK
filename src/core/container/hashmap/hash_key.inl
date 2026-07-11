#pragma once

#include "hash_key.hpp"
#include "defines.h"
#include "wyhash.hpp"
#include "core/types/vertex.h"
#include "glm/glm.hpp"

#include <bit>
#include <string>

template<typename T>
SV_FORCE_INLINE void hash_key_combine(u64& seed, const T& value)
{
    seed ^= hash_key<T>{}(value) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

// ---- integer types ----
template<>
struct hash_key<u64> {
    u64 operator()(u64 key) const {
        key ^= key >> 33;
        key *= 0xff51afd7ed558ccdULL;
        key ^= key >> 33;
        key *= 0xc4ceb9fe1a85ec53ULL;
        key ^= key >> 33;
        return key;
    }
};

template<>
struct hash_key<u8> {
    u64 operator()(u8 key) const { return hash_key<u64>{}(static_cast<u64>(key)); }
};

template<>
struct hash_key<u16> {
    u64 operator()(u16 key) const { return hash_key<u64>{}(static_cast<u64>(key)); }
};

template<>
struct hash_key<u32> {
    u64 operator()(u32 key) const { return hash_key<u64>{}(static_cast<u64>(key)); }
};

template<>
struct hash_key<i8> {
    u64 operator()(i8 key) const { return hash_key<u64>{}(static_cast<u64>(key)); }
};

template<>
struct hash_key<i16> {
    u64 operator()(i16 key) const { return hash_key<u64>{}(static_cast<u64>(key)); }
};

template<>
struct hash_key<i32> {
    u64 operator()(i32 key) const { return hash_key<u64>{}(static_cast<u64>(key)); }
};

template<>
struct hash_key<i64> {
    u64 operator()(i64 key) const { return hash_key<u64>{}(static_cast<u64>(key)); }
};

// ---- float types ----
template<> struct hash_key<f32> {
    u64 operator()(f32 key) const { return hash_key<u64>{}(std::bit_cast<u32>(key)); }
};

template<> struct hash_key<f64> {
    u64 operator()(f64 key) const { return hash_key<u64>{}(std::bit_cast<u64>(key)); }
};

// ---- string types ----
template<u64 N>
struct hash_key<const char[N]> {
    u64 operator()(const char (&str)[N]) const {
        return wyhash(str, N - 1, 0, _wyp);
    }
};

template<>
struct hash_key<String> {
    u64 operator()(const std::string& str) const {
        return wyhash(str.data(), str.size(), 0, _wyp);
    }
};

// ---- pointer ----

template<typename T>
u64 hash_key<T*>::operator()(const T* key) const {
    return hash_key<u64>{}(reinterpret_cast<u64>(key));
}

template<>
struct hash_key<glm::vec1> {
    u64 operator()(const glm::vec1& v) const
    {
        u64 seed = 0;

        hash_key_combine(seed, v.x);

        return seed;
    }
};

template<>
struct hash_key<glm::vec2> {
    u64 operator()(const glm::vec2& v) const
    {
        u64 seed = 0;

        hash_key_combine(seed, v.x);
        hash_key_combine(seed, v.y);

        return seed;
    }
};

template<>
struct hash_key<glm::vec3> {
    u64 operator()(const glm::vec3& v) const
    {
        u64 seed = 0;

        hash_key_combine(seed, v.x);
        hash_key_combine(seed, v.y);
        hash_key_combine(seed, v.z);

        return seed;
    }
};

template<>
struct hash_key<glm::vec4> {
    u64 operator()(const glm::vec4& v) const
    {
        u64 seed = 0;

        hash_key_combine(seed, v.x);
        hash_key_combine(seed, v.y);
        hash_key_combine(seed, v.z);
        hash_key_combine(seed, v.w);

        return seed;
    }
};

