#pragma once

#include "hashmap.hpp"
#include "hash_key.hpp"
#include "debug/debugger.h"
#include <utility>

// ---- helpers: swap / move across all parallel arrays (incl fingerprints) ----
template <typename K, typename V, typename Entry>
SV_FORCE_INLINE static void swap_slot(
    Array<K>& keys, Array<V>& vals, Array<u64>& hashes,
    Array<u64>& dists, Array<b32>& occ, Array<u8>& fps, u64 a, u64 b)
{
    std::swap(keys[a],   keys[b]);
    std::swap(vals[a],   vals[b]);
    std::swap(hashes[a], hashes[b]);
    std::swap(dists[a],  dists[b]);
    std::swap(occ[a],    occ[b]);
    std::swap(fps[a],    fps[b]);
}

template <typename K, typename V, typename Entry>
SV_FORCE_INLINE static void move_slot(
    Array<K>& keys, Array<V>& vals, Array<u64>& hashes,
    Array<u64>& dists, Array<b32>& occ, Array<u8>& fps, u64 dst, u64 src)
{
    keys[dst]   = std::move(keys[src]);
    vals[dst]   = std::move(vals[src]);
    hashes[dst] = hashes[src];
    dists[dst]  = dists[src];
    occ[dst]    = occ[src];
    fps[dst]    = fps[src];
}

// =========================================================================
// Constructor / move / operator=
// =========================================================================

template <typename K, typename V, typename Entry>
HashMap<K, V, Entry>::HashMap(u64 capacity, Allocator* a)
    : m_allocator(a), m_keys(a), m_vals(a),
      m_hashKeys(a), m_probeDists(a), m_occupied(a), m_fingerprints(a)
{
    m_capacity = next_pow2(capacity);
    m_mask     = m_capacity - 1;
    m_keys.resize(m_capacity);
    m_vals.resize(m_capacity);
    m_hashKeys.resize(m_capacity);
    m_probeDists.resize(m_capacity);
    m_occupied.resize(m_capacity);
    m_fingerprints.resize(m_capacity);
    for (u64 i = 0; i < m_capacity; ++i) m_fingerprints[i] = 0;
}

template <typename K, typename V, typename Entry>
HashMap<K, V, Entry>::HashMap(HashMap<K, V, Entry>&& other) noexcept
    : m_allocator(other.m_allocator)
    , m_size(other.m_size)
    , m_capacity(other.m_capacity)
    , m_mask(other.m_mask)
    , m_keys(std::move(other.m_keys))
    , m_vals(std::move(other.m_vals))
    , m_hashKeys(std::move(other.m_hashKeys))
    , m_probeDists(std::move(other.m_probeDists))
    , m_occupied(std::move(other.m_occupied))
    , m_fingerprints(std::move(other.m_fingerprints))
{
    other.m_allocator = nullptr;
    other.m_size      = 0;
    other.m_capacity  = 0;
    other.m_mask      = 0;
}

template <typename K, typename V, typename Entry>
HashMap<K, V, Entry>& HashMap<K, V, Entry>::operator=(HashMap<K, V, Entry>&& other) noexcept {
    if (this != &other) {
        m_keys         = std::move(other.m_keys);
        m_vals         = std::move(other.m_vals);
        m_hashKeys     = std::move(other.m_hashKeys);
        m_probeDists   = std::move(other.m_probeDists);
        m_occupied     = std::move(other.m_occupied);
        m_fingerprints = std::move(other.m_fingerprints);
        m_allocator    = other.m_allocator;
        m_size         = other.m_size;
        m_capacity     = other.m_capacity;
        m_mask         = other.m_mask;

        other.m_allocator = nullptr;
        other.m_size      = 0;
        other.m_capacity  = 0;
        other.m_mask      = 0;
    }
    return *this;
}

template <typename K, typename V, typename Entry>
HashMap<K, V, Entry>::~HashMap() {}

// =========================================================================
// Reserve
// =========================================================================

template <typename K, typename V, typename Entry>
void HashMap<K, V, Entry>::reserve(u64 capacity) {
    if (capacity > m_capacity) rehash(capacity);
}

// =========================================================================
// Insert forwarders
// =========================================================================

template <typename K, typename V, typename Entry>
void HashMap<K, V, Entry>::insert(const K& key, const V& val) { insert_impl(key, val); }
template <typename K, typename V, typename Entry>
void HashMap<K, V, Entry>::insert(const K& key, V&& val)      { insert_impl(key, std::move(val)); }
template <typename K, typename V, typename Entry>
void HashMap<K, V, Entry>::insert(K&& key, const V& val)      { insert_impl(std::move(key), val); }
template <typename K, typename V, typename Entry>
void HashMap<K, V, Entry>::insert(K&& key, V&& val)           { insert_impl(std::move(key), std::move(val)); }

// =========================================================================
// Emplace
// =========================================================================

template <typename K, typename V, typename Entry>
template <class... Args>
typename HashMap<K, V, Entry>::iterator
HashMap<K, V, Entry>::emplace(Args&&... args) {
    Entry entry(std::forward<Args>(args)...);
    insert(std::move(entry));

    u64 fp    = make_fingerprint(entry.hash);
    u64 index = entry.hash & m_mask;
    u64 group = group_start(index);
    u64 dist  = 0;

    while (true) {
        u64 offset  = index & GROUP_MASK;
        u64* fps    = reinterpret_cast<u64*>(m_fingerprints.data() + group);
        u64 fp_mask = *fps;  // load first 8 fingerprints
        // ... not group-simd for emplace (rarely used), just linear scan
        for (u64 i = offset; i < GROUP_SIZE; ++i) {
            u64 idx = group + i;
            if (m_occupied[idx] && m_fingerprints[idx] == fp &&
                m_hashKeys[idx] == entry.hash && m_keys[idx] == entry.key)
                return make_iter(idx);
            if (!m_occupied[idx] || m_probeDists[idx] < dist)
                return end();
            ++dist;
        }
        group = (group + GROUP_SIZE) & m_mask;
        index = group;
    }
}

// =========================================================================
// Find  (group probing with SIMD fingerprint match)
// =========================================================================

template <typename K, typename V, typename Entry>
typename HashMap<K, V, Entry>::iterator
HashMap<K, V, Entry>::find(const K& key) {
    if (m_size == 0) return end();

    u64 hashVal = hash_key<K>{}(key);
    u8  fp      = make_fingerprint(hashVal);
    u64 index   = hashVal & m_mask;
    u64 group   = group_start(index);

    while (true) {
        u32 match_mask = match_group(m_fingerprints.data() + group, fp);

        // only look at positions at or after our probe start within this group
        u64 off  = index & GROUP_MASK;
        match_mask &= ~((1u << off) - 1);

        while (match_mask) {
            u64 bit = __builtin_ctz(match_mask);
            u64 idx = group + bit;
            if (m_hashKeys[idx] == hashVal && m_keys[idx] == key)
                return make_iter(idx);
            match_mask &= match_mask - 1;
        }

        // if any slot in this group (from probe start onward) is empty, key absent
        for (u64 i = off; i < GROUP_SIZE; ++i) {
            if (!m_occupied[group + i])
                return end();
        }

        group = (group + GROUP_SIZE) & m_mask;
        index = group;
    }
}

template <typename K, typename V, typename Entry>
typename HashMap<K, V, Entry>::const_iterator
HashMap<K, V, Entry>::find(const K& key) const {
    if (m_size == 0) return end();

    u64 hashVal = hash_key<K>{}(key);
    u8  fp      = make_fingerprint(hashVal);
    u64 index   = hashVal & m_mask;
    u64 group   = group_start(index);

    while (true) {
        u32 match_mask = match_group(m_fingerprints.data() + group, fp);
        u64 off  = index & GROUP_MASK;
        match_mask &= ~((1u << off) - 1);

        while (match_mask) {
            u64 bit = __builtin_ctz(match_mask);
            u64 idx = group + bit;
            if (m_hashKeys[idx] == hashVal && m_keys[idx] == key)
                return make_citer(idx);
            match_mask &= match_mask - 1;
        }

        for (u64 i = off; i < GROUP_SIZE; ++i)
            if (!m_occupied[group + i]) return end();

        group = (group + GROUP_SIZE) & m_mask;
        index = group;
    }
}

// =========================================================================
// Contains
// =========================================================================

template <typename K, typename V, typename Entry>
b32 HashMap<K, V, Entry>::contains(const K& key) {
    return find(key) != end();
}

// =========================================================================
// operator[]
// =========================================================================

template <typename K, typename V, typename Entry>
V& HashMap<K, V, Entry>::operator[](const K& key) {
    auto it = find(key);
    if (it != end()) return m_vals[it.index()];
    insert(key, V{});
    return m_vals[find(key).index()];
}

template <typename K, typename V, typename Entry>
const V& HashMap<K, V, Entry>::operator[](const K& key) const {
    auto it = find(key);
    if (it != end()) return m_vals[it.index()];
    FATAL("Key not found in const HashMap::operator[]");
    failure_exit();
}

// =========================================================================
// Erase — group probing + backward-shift
// =========================================================================

template <typename K, typename V, typename Entry>
void HashMap<K, V, Entry>::erase(const K& key) {
    if (m_size == 0) return;

    u64 hashVal = hash_key<K>{}(key);
    u8  fp      = make_fingerprint(hashVal);
    u64 index   = hashVal & m_mask;
    u64 group   = group_start(index);

    while (true) {
        u32 match_mask = match_group(m_fingerprints.data() + group, fp);
        u64 off = index & GROUP_MASK;
        match_mask &= ~((1u << off) - 1);

        while (match_mask) {
            u64 bit = __builtin_ctz(match_mask);
            u64 idx = group + bit;
            if (m_hashKeys[idx] == hashVal && m_keys[idx] == key) {
                m_occupied[idx] = SV_FALSE;
                m_fingerprints[idx] = 0;
                --m_size;

                // backward shift
                u64 next = (idx + 1) & m_mask;
                while (m_occupied[next] && m_probeDists[next] > 0) {
                    move_slot<K,V,Entry>(m_keys, m_vals, m_hashKeys,
                        m_probeDists, m_occupied, m_fingerprints, idx, next);
                    m_probeDists[idx]--;
                    m_occupied[next] = SV_FALSE;
                    m_fingerprints[next] = 0;
                    idx = next;
                    next = (idx + 1) & m_mask;
                }
                return;
            }
            match_mask &= match_mask - 1;
        }

        for (u64 i = off; i < GROUP_SIZE; ++i)
            if (!m_occupied[group + i]) return;

        group = (group + GROUP_SIZE) & m_mask;
        index = group;
    }
}

// =========================================================================
// Clear
// =========================================================================

template <typename K, typename V, typename Entry>
void HashMap<K, V, Entry>::clear() {
    for (u64 i = 0; i < m_capacity; ++i) {
        m_occupied[i]     = SV_FALSE;
        m_fingerprints[i] = 0;
    }
    m_size = 0;
}

// =========================================================================
// Rehash
// =========================================================================

template <typename K, typename V, typename Entry>
void HashMap<K, V, Entry>::rehash(u64 newCapacity) {
    newCapacity = next_pow2(newCapacity);

    Array<K>   oldKeys   = std::move(m_keys);
    Array<V>   oldVals   = std::move(m_vals);
    Array<u64> oldHashes = std::move(m_hashKeys);
    Array<u64> oldDists  = std::move(m_probeDists);
    Array<b32> oldOcc    = std::move(m_occupied);
    u64 oldCapacity      = m_capacity;

    m_capacity = newCapacity;
    m_mask     = m_capacity - 1;
    m_keys         = Array<K>(m_allocator);   m_keys.resize(m_capacity);
    m_vals         = Array<V>(m_allocator);   m_vals.resize(m_capacity);
    m_hashKeys     = Array<u64>(m_allocator); m_hashKeys.resize(m_capacity);
    m_probeDists   = Array<u64>(m_allocator); m_probeDists.resize(m_capacity);
    m_occupied     = Array<b32>(m_allocator); m_occupied.resize(m_capacity);
    m_fingerprints = Array<u8>(m_allocator);  m_fingerprints.resize(m_capacity);
    for (u64 i = 0; i < m_capacity; ++i) m_fingerprints[i] = 0;
    m_size = 0;

    for (u64 i = 0; i < oldCapacity; ++i) {
        if (!oldOcc[i]) continue;

        u64 hashVal = oldHashes[i];
        u8  fp      = make_fingerprint(hashVal);
        u64 probe   = 0;
        u64 index   = hashVal & m_mask;

        while (true) {
            if (!m_occupied[index]) {
                m_keys[index]         = std::forward<K>(oldKeys[i]);
                m_vals[index]         = std::forward<V>(oldVals[i]);
                m_hashKeys[index]     = hashVal;
                m_probeDists[index]   = probe;
                m_occupied[index]     = SV_TRUE;
                m_fingerprints[index] = fp;
                ++m_size;
                break;
            }

            if (m_fingerprints[index] == fp && m_hashKeys[index] == hashVal
                && m_keys[index] == oldKeys[i]) {
                m_vals[index] = std::forward<V>(oldVals[i]);
                break;
            }

            if (m_probeDists[index] < probe) {
                std::swap(m_keys[index],         oldKeys[i]);
                std::swap(m_vals[index],         oldVals[i]);
                std::swap(m_hashKeys[index],     hashVal);
                std::swap(m_probeDists[index],   probe);
                std::swap(m_fingerprints[index], fp);
            }

            ++probe;
            index = (index + 1) & m_mask;
        }
    }
}

// =========================================================================
// insert_impl  (group probing + Robin Hood)
// =========================================================================

template <typename K, typename V, typename Entry>
template <typename Key, typename Val>
void HashMap<K, V, Entry>::insert_impl(Key&& key, Val&& val) {
    if ((m_size + 1) >= static_cast<u64>(static_cast<f64>(m_capacity) * static_cast<f64>(REHASH_FACTOR))) {
        rehash(m_capacity * 2);
    }

    u64 hashVal = hash_key<std::remove_cvref_t<K>>{}(key);
    u8  fp      = make_fingerprint(hashVal);
    u64 index   = hashVal & m_mask;

    K   curKey   = std::forward<Key>(key);
    V   curVal   = std::forward<Val>(val);
    u64 curHash  = hashVal;
    u8  curFp    = fp;
    u64 curProbe = 0;

    // group: step through groups, scanning within each
    while (true) {
        u64 group = group_start(index);
        u64 off   = index & GROUP_MASK;

        for (u64 i = off; i < GROUP_SIZE; ++i) {
            u64 idx = group + i;

            if (!m_occupied[idx]) {
                m_keys[idx]         = std::move(curKey);
                m_vals[idx]         = std::move(curVal);
                m_hashKeys[idx]     = curHash;
                m_fingerprints[idx] = curFp;
                m_probeDists[idx]   = curProbe;
                m_occupied[idx]     = SV_TRUE;
                ++m_size;
                return;
            }

            if (m_fingerprints[idx] == curFp && m_hashKeys[idx] == curHash
                && m_keys[idx] == curKey) {
                m_vals[idx] = std::move(curVal);
                return;
            }

            if (m_probeDists[idx] < curProbe) {
                std::swap(m_keys[idx],         curKey);
                std::swap(m_vals[idx],         curVal);
                std::swap(m_hashKeys[idx],     curHash);
                std::swap(m_fingerprints[idx], curFp);
                std::swap(m_probeDists[idx],   curProbe);
            }

            ++curProbe;
        }

        group = (group + GROUP_SIZE) & m_mask;
        index = group;
    }
}
