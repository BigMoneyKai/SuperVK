#pragma once

#include "defines.h"
#include "hash_entry.hpp"

template <typename Entry>
class HashMapIterator {
    using K = typename Entry::KeyType;
    using V = typename Entry::ValType;
    using NK = std::remove_const_t<K>;
    using NV = std::remove_const_t<V>;

  public:
    HashMapIterator(u64 idx, u64 end,
                    NK* keys, NV* vals, u64* hashes, u64* dists, b32* occ)
        : m_idx(idx), m_end(end),
          m_keys(keys), m_vals(vals),
          m_hashes(hashes), m_dists(dists), m_occupied(occ) {}

    const Entry& operator*()  const { sync(); return reinterpret_cast<const Entry&>(m_cache); }
    const Entry* operator->() const { sync(); return reinterpret_cast<const Entry*>(&m_cache); }

    HashMapIterator& operator++() {
        ++m_idx;
        while (m_idx != m_end && !m_occupied[m_idx])
            ++m_idx;
        return *this;
    }

    HashMapIterator operator++(int) {
        HashMapIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    SV_FORCE_INLINE NK& first() {
        return m_keys[m_idx];
    }
    SV_FORCE_INLINE const NK& first() const {
        return m_keys[m_idx];
    }

    SV_FORCE_INLINE NV& second() {
        return m_vals[m_idx];
    }
    SV_FORCE_INLINE const NV& second() const {
        return m_vals[m_idx];
    }

    bool operator==(const HashMapIterator& other) const { return m_idx == other.m_idx; }
    bool operator!=(const HashMapIterator& other) const { return m_idx != other.m_idx; }

    u64 index() const { return m_idx; }   // for find() to construct result iterators

  private:
    void sync() const {
        m_cache.key       = m_keys[m_idx];
        m_cache.val       = m_vals[m_idx];
        m_cache.hash      = m_hashes[m_idx];
        m_cache.probeDist = m_dists[m_idx];
        m_cache.occupied  = m_occupied[m_idx];
    }

    u64   m_idx;
    u64   m_end;
    NK*   m_keys;
    NV*   m_vals;
    u64*  m_hashes;
    u64*  m_dists;
    b32*  m_occupied;
    mutable HashEntry<NK, NV> m_cache;
};
