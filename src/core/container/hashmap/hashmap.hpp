#pragma once

#include "core/container/array/array.hpp"
#include "core/memory/allocator.h"
#include "hash_entry.hpp"
#include "hash_key.hpp"
#include "hashmap_iterator.hpp"
#include "simd_group.hpp"

#include <utility>

template <typename K, typename V, typename Entry = HashEntry<K, V>>
class HashMap {
public:
  using iterator = HashMapIterator<Entry>;
  using const_iterator = const HashMapIterator<Entry>;

  HashMap() = delete;
  HashMap(const HashMap &) = delete;
  HashMap &operator=(const HashMap &) = delete;

  explicit HashMap(u64 capacity, Allocator *a = &default_allocator());
  HashMap(HashMap &&other) noexcept;
  HashMap &operator=(HashMap &&other) noexcept;

  ~HashMap();

  void reserve(u64 capacity);

  void insert(const K &key, const V &val);
  void insert(const K &key, V &&val);
  void insert(K &&key, const V &val);
  void insert(K &&key, V &&val);
  void insert(const Entry &entry);
  void insert(Entry &&entry);

  template <class... Args> iterator emplace(Args &&...args);

  iterator find(const K &key);
  const_iterator find(const K &key) const;

  void erase(const K &key);

  void clear();
  void rehash(u64 newCapacity);
  b32 contains(const K &key) const;

  V &operator[](const K &key);
  const V &operator[](const K &key) const;

  SV_FORCE_INLINE iterator begin() {
    auto *k = m_keys.data();
    auto *v = m_vals.data();
    auto *h = m_hashKeys.data();
    auto *d = m_probeDists.data();
    auto *o = m_occupied.data();
    auto ei = m_capacity;
    u64 i = 0;
    while (i != ei && !o[i])
      ++i;
    return iterator(i, ei, k, v, h, d, o);
  }
  SV_FORCE_INLINE const_iterator begin() const {
    auto *k = m_keys.data();
    auto *v = m_vals.data();
    auto *h = m_hashKeys.data();
    auto *d = m_probeDists.data();
    auto *o = m_occupied.data();
    auto ei = m_capacity;
    u64 i = 0;
    while (i != ei && !o[i])
      ++i;
    return const_iterator(i, ei, k, v, h, d, o);
  }
  SV_FORCE_INLINE iterator end() {
    auto ei = m_capacity;
    auto *k = m_keys.data();
    auto *v = m_vals.data();
    auto *h = m_hashKeys.data();
    auto *d = m_probeDists.data();
    auto *o = m_occupied.data();
    return iterator(ei, ei, k, v, h, d, o);
  }
  SV_FORCE_INLINE const_iterator end() const {
    auto ei = m_capacity;
    auto *k = m_keys.data();
    auto *v = m_vals.data();
    auto *h = m_hashKeys.data();
    auto *d = m_probeDists.data();
    auto *o = m_occupied.data();
    return const_iterator(ei, ei, k, v, h, d, o);
  }

  SV_FORCE_INLINE u64 size() const { return m_size; }
  SV_FORCE_INLINE u64 capacity() const { return m_capacity; }
  SV_FORCE_INLINE b32 empty() const { return m_size == 0; }

private:
  template <typename Key, typename Val> void insert_impl(Key &&key, Val &&val);

  SV_FORCE_INLINE static u64 next_pow2(u64 v) {
    v = v < GROUP_SIZE ? GROUP_SIZE : v;
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    return v + 1;
  }

  SV_FORCE_INLINE iterator make_iter(u64 idx) {
    return iterator(idx, m_capacity, m_keys.data(), m_vals.data(),
                    m_hashKeys.data(), m_probeDists.data(), m_occupied.data());
  }
  SV_FORCE_INLINE const_iterator make_citer(u64 idx) const {
    return const_iterator(idx, m_capacity, m_keys.data(), m_vals.data(),
                          m_hashKeys.data(), m_probeDists.data(),
                          m_occupied.data());
  }

  // group-probe helpers
  SV_FORCE_INLINE u64 group_start(u64 idx) const { return idx & ~GROUP_MASK; }
  SV_FORCE_INLINE bool group_has_empty(u64 g) const {
    for (u64 i = 0; i < GROUP_SIZE; ++i)
      if (!m_occupied[g + i])
        return true;
    return false;
  }

private:
  static constexpr f32 REHASH_FACTOR = 0.875f;
  static constexpr f32 GROWTH_FACTOR = 2.0f;

  Allocator *m_allocator{nullptr};
  u64 m_size{0};
  u64 m_capacity{0};
  u64 m_mask{0};

  Array<K> m_keys;
  Array<V> m_vals;
  Array<u64> m_hashKeys;
  Array<u64> m_probeDists;
  Array<b32> m_occupied;
  Array<u8> m_fingerprints; // 1-byte hash fingerprint per slot
};

#include "hashmap.inl"
