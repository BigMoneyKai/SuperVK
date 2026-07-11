#pragma once
#include "defines.h"
#include <utility>

template <typename K, typename V>
struct HashEntry {
    using KeyType = K;
    using ValType = V;

    K   key{};
    V   val{};
    u64 hash{0};
    u64 probeDist{0};
    b32 occupied{SV_FALSE};

    HashEntry() = default;

    HashEntry(const K& k, const V& v, u64 h = 0, u64 dib = 0, b32 occ = SV_FALSE)
        : key(k), val(v), hash(h), probeDist(dib), occupied(occ) {}

    HashEntry(const K& k, V&& v, u64 h = 0, u64 dib = 0, b32 occ = SV_FALSE)
        : key(k), val(std::move(v)), hash(h), probeDist(dib), occupied(occ) {}

    HashEntry(K&& k, const V& v, u64 h = 0, u64 dib = 0, b32 occ = SV_FALSE)
        : key(std::move(k)), val(v), hash(h), probeDist(dib), occupied(occ) {}

    HashEntry(K&& k, V&& v, u64 h = 0, u64 dib = 0, b32 occ = SV_FALSE)
        : key(std::move(k)), val(std::move(v)), hash(h), probeDist(dib), occupied(occ) {}
};
