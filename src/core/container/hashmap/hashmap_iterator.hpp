#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"
#include "hash_entry.hpp"

template <typename Entry> class HashMapIterator {
  using K = typename Entry::KeyType;
  using V = typename Entry::ValType;
  using NK = std::remove_const_t<K>;
  using NV = std::remove_const_t<V>;

public:
  HashMapIterator(u64 idx, u64 end, const NK *keys, const NV *vals,
                  const u64 *hashes, const u64 *dists, const u8 *fingerprints)
      : m_idx(idx), m_end(end), m_keys(keys), m_vals(vals), m_hashes(hashes),
        m_dists(dists), m_fingerprints(fingerprints) {}

  const Entry &operator*() const {
    sync();
    return reinterpret_cast<const Entry &>(m_cache);
  }
  const Entry *operator->() const {
    sync();
    return reinterpret_cast<const Entry *>(&m_cache);
  }

  HashMapIterator &operator++() {
    ++m_idx;
    while (m_idx != m_end && !m_fingerprints[m_idx])
      ++m_idx;
    return *this;
  }

  HashMapIterator operator++(int) {
    HashMapIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  SV_FORCE_INLINE NK &first() { return const_cast<NK *>(m_keys)[m_idx]; }
  SV_FORCE_INLINE const NK &first() const { return m_keys[m_idx]; }

  SV_FORCE_INLINE NV &second() { return const_cast<NV *>(m_vals)[m_idx]; }
  SV_FORCE_INLINE const NV &second() const { return m_vals[m_idx]; }

  bool operator==(const HashMapIterator &other) const {
    return m_idx == other.m_idx;
  }
  bool operator!=(const HashMapIterator &other) const {
    return m_idx != other.m_idx;
  }

  u64 index() const { return m_idx; }

private:
  void sync() const {
    m_cache.key = m_keys[m_idx];
    m_cache.val = m_vals[m_idx];
    m_cache.hash = m_hashes[m_idx];
    m_cache.probeDist = m_dists[m_idx];
    m_cache.fingerprint = m_fingerprints[m_idx];
  }

  u64 m_idx;
  u64 m_end;
  const NK *m_keys;
  const NV *m_vals;
  const u64 *m_hashes;
  const u64 *m_dists;
  const u8 *m_fingerprints;
  mutable HashEntry<NK, NV> m_cache;
};
