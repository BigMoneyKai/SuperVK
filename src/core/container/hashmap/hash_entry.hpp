#pragma once
#include "core/define/types.h"
#include <utility>

template <typename K, typename V> struct HashEntry {
  using KeyType = K;
  using ValType = V;

  K key{};
  V val{};
  u64 hash{0};
  u64 probeDist{0};
  u8 fingerprint{0};

  HashEntry() = default;

  HashEntry(const K &k, const V &v, u64 h = 0, u64 dib = 0, u8 fp = 0)
      : key(k), val(v), hash(h), probeDist(dib), fingerprint(fp) {}

  HashEntry(const K &k, V &&v, u64 h = 0, u64 dib = 0, u8 fp = 0)
      : key(k), val(std::move(v)), hash(h), probeDist(dib), fingerprint(fp) {}

  HashEntry(K &&k, const V &v, u64 h = 0, u64 dib = 0, u8 fp = 0)
      : key(std::move(k)), val(v), hash(h), probeDist(dib), fingerprint(fp) {}

  HashEntry(K &&k, V &&v, u64 h = 0, u64 dib = 0, u8 fp = 0)
      : key(std::move(k)), val(std::move(v)), hash(h), probeDist(dib),
        fingerprint(fp) {}
};
