#pragma once

#include "platform/memory.h"
#include "ring_buffer.hpp"
#include <type_traits>
#include <utility>

// ============================================================================
// Helpers
// ============================================================================

template <typename T> T *RingBuffer<T>::alloc_buffer(u64 count) {
  if (m_allocator)
    return static_cast<T *>(
        m_allocator->allocate(count * sizeof(T), ALIGNMENT));
  return static_cast<T *>(platform_aligned_alloc(count * sizeof(T), ALIGNMENT));
}

template <typename T> void RingBuffer<T>::free_buffer(T *ptr) {
  if (m_allocator)
    m_allocator->deallocate(ptr);
  else
    platform_aligned_free(ptr);
}

template <typename T> void RingBuffer<T>::destroy_elements() {
  if constexpr (!std::is_trivially_destructible_v<T>) {
    for (u64 i = 0; i < m_size; ++i)
      m_data[(m_head + i) & m_mask].~T();
  }
}

// ============================================================================
// Constructors / move / destructor
// ============================================================================

template <typename T>
RingBuffer<T>::RingBuffer(Allocator *a) : m_allocator(a) {}

template <typename T>
RingBuffer<T>::RingBuffer(u64 capacity, const T &value, Allocator *a)
    : m_allocator(a) {
  if (capacity == 0)
    return;

  m_capacity = next_pow2(capacity);
  m_mask = m_capacity - 1;
  m_data = alloc_buffer(m_capacity);

  for (u64 i = 0; i < capacity; ++i)
    new (m_data + i) T(value);

  m_size = capacity;
  m_head = 0;
  m_tail = capacity & m_mask;
}

template <typename T>
RingBuffer<T>::RingBuffer(RingBuffer<T> &&other) noexcept
    : m_allocator(other.m_allocator), m_data(other.m_data),
      m_size(other.m_size), m_capacity(other.m_capacity), m_mask(other.m_mask),
      m_head(other.m_head), m_tail(other.m_tail) {
  other.m_allocator = nullptr;
  other.m_data = nullptr;
  other.m_size = 0;
  other.m_capacity = 0;
  other.m_mask = 0;
  other.m_head = 0;
  other.m_tail = 0;
}

template <typename T>
RingBuffer<T> &RingBuffer<T>::operator=(RingBuffer<T> &&other) noexcept {
  if (this != &other) {
    destroy_elements();
    if (m_data)
      free_buffer(m_data);

    m_allocator = other.m_allocator;
    m_data = other.m_data;
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_mask = other.m_mask;
    m_head = other.m_head;
    m_tail = other.m_tail;

    other.m_allocator = nullptr;
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
    other.m_mask = 0;
    other.m_head = 0;
    other.m_tail = 0;
  }
  return *this;
}

template <typename T> RingBuffer<T>::~RingBuffer() {
  if (!m_data)
    return;
  destroy_elements();
  free_buffer(m_data);
}

// ============================================================================
// reserve — allocate new buffer, linearize circular data, swap
// ============================================================================

template <typename T> void RingBuffer<T>::reserve(u64 newCapacity) {
  if (newCapacity <= m_capacity)
    return;

  newCapacity = next_pow2(newCapacity);
  T *newData = alloc_buffer(newCapacity);
  u64 newMask = newCapacity - 1;

  // copy from head, wrapping, for size elements → linear [0..size)
  for (u64 i = 0; i < m_size; ++i) {
    u64 src = (m_head + i) & m_mask;
    new (newData + i) T(std::move_if_noexcept(m_data[src]));
  }

  destroy_elements();
  if (m_data)
    free_buffer(m_data);

  m_data = newData;
  m_capacity = newCapacity;
  m_mask = newMask;
  m_head = 0;
  m_tail = m_size & m_mask; // wraps to 0 when full
}

// ============================================================================
// resize — grow/shrink logical element count
// ============================================================================

template <typename T> void RingBuffer<T>::resize(u64 newSize, const T &value) {
  if (newSize > m_capacity) {
    // grow buffer (1.5x heuristic), then fill
    u64 growCap = m_capacity + (m_capacity >> 1);
    if (growCap < newSize)
      growCap = newSize;
    reserve(growCap);
  }

  if (newSize > m_size) {
    // append value-initialized elements at tail
    for (u64 i = m_size; i < newSize; ++i) {
      u64 pos = (m_head + i) & m_mask;
      new (m_data + pos) T(value);
    }
  } else if (newSize < m_size) {
    // destroy trailing elements from the back
    if constexpr (!std::is_trivially_destructible_v<T>) {
      for (u64 i = newSize; i < m_size; ++i)
        m_data[(m_head + i) & m_mask].~T();
    }
  }

  m_tail = (m_head + newSize) & m_mask;
  m_size = newSize;
}

// ============================================================================
// shrink_to_fit — reduce capacity to next_pow2(size)
// ============================================================================

template <typename T> void RingBuffer<T>::shrink_to_fit() {
  u64 target = next_pow2(m_size);
  if (target >= m_capacity)
    return;

  T *newData = alloc_buffer(target);
  u64 newMask = target - 1;

  for (u64 i = 0; i < m_size; ++i) {
    u64 src = (m_head + i) & m_mask;
    new (newData + i) T(std::move_if_noexcept(m_data[src]));
  }

  destroy_elements();
  free_buffer(m_data);

  m_data = newData;
  m_capacity = target;
  m_mask = newMask;
  m_head = 0;
  m_tail = m_size & m_mask;
}

// ============================================================================
// operator[] — logical index (0 = front, size-1 = back)
// ============================================================================

template <typename T> T &RingBuffer<T>::operator[](u64 i) {
  SV_ASSERT(i < m_size, "RingBuffer index {} out of bounds (size={})", i,
            m_size);
  return m_data[(m_head + i) & m_mask];
}

template <typename T> const T &RingBuffer<T>::operator[](u64 i) const {
  SV_ASSERT(i < m_size, "RingBuffer index {} out of bounds (size={})", i,
            m_size);
  return m_data[(m_head + i) & m_mask];
}
template <typename T> T &RingBuffer<T>::at(u64 i) {
  SV_ASSERT(i < m_size, "RingBuffer index {} out of bounds (size={})", i,
            m_size);
  return m_data[(m_head + i) & m_mask];
}
template <typename T> const T &RingBuffer<T>::at(u64 i) const {
  SV_ASSERT(i < m_size, "RingBuffer index {} out of bounds (size={})", i,
            m_size);
  return m_data[(m_head + i) & m_mask];
}
// ============================================================================
// push_back — place at tail, grow if full
// ============================================================================

template <typename T> void RingBuffer<T>::push_back(const T &value) {
  if (m_size >= m_capacity)
    reserve(m_capacity == 0 ? 2 : m_capacity * 2);

  new (m_data + m_tail) T(value);
  m_tail = (m_tail + 1) & m_mask;
  ++m_size;
}

template <typename T> void RingBuffer<T>::push_back(T &&value) {
  if (m_size >= m_capacity)
    reserve(m_capacity == 0 ? 2 : m_capacity * 2);

  new (m_data + m_tail) T(std::move(value));
  m_tail = (m_tail + 1) & m_mask;
  ++m_size;
}

// ============================================================================
// emplace_back — in-place construction at tail
// ============================================================================

template <typename T>
template <typename... Args>
T &RingBuffer<T>::emplace_back(Args &&...args) {
  if (m_size >= m_capacity)
    reserve(m_capacity == 0 ? 2 : m_capacity * 2);

  T *obj = new (m_data + m_tail) T(std::forward<Args>(args)...);
  m_tail = (m_tail + 1) & m_mask;
  ++m_size;
  return *obj;
}

// ============================================================================
// pop_front — destroy at head, advance
// ============================================================================

template <typename T> void RingBuffer<T>::pop_front() {
  if (m_size == 0)
    return;

  if constexpr (!std::is_trivially_destructible_v<T>)
    m_data[m_head].~T();

  m_head = (m_head + 1) & m_mask;
  --m_size;
}

// ============================================================================
// insert — place at logical index, shift tail right
// ============================================================================

template <typename T> void RingBuffer<T>::insert(u64 index, const T &value) {
  SV_ASSERT(index <= m_size,
            "RingBuffer insert index {} out of bounds (size={})", index,
            m_size);

  if (m_size >= m_capacity)
    reserve(m_capacity == 0 ? 2 : m_capacity * 2);

  // shift [index .. size-1] right by one (walk backwards to avoid clobbering)
  for (u64 i = m_size; i > index; --i) {
    u64 dst = (m_head + i) & m_mask;
    u64 src = (m_head + i - 1) & m_mask;
    new (m_data + dst) T(std::move_if_noexcept(m_data[src]));
    if constexpr (!std::is_trivially_destructible_v<T>)
      m_data[src].~T();
  }

  u64 pos = (m_head + index) & m_mask;
  new (m_data + pos) T(value);
  m_tail = (m_tail + 1) & m_mask;
  ++m_size;
}

template <typename T> void RingBuffer<T>::insert(u64 index, T &&value) {
  SV_ASSERT(index <= m_size,
            "RingBuffer insert index {} out of bounds (size={})", index,
            m_size);

  if (m_size >= m_capacity)
    reserve(m_capacity == 0 ? 2 : m_capacity * 2);

  for (u64 i = m_size; i > index; --i) {
    u64 dst = (m_head + i) & m_mask;
    u64 src = (m_head + i - 1) & m_mask;
    new (m_data + dst) T(std::move_if_noexcept(m_data[src]));
    if constexpr (!std::is_trivially_destructible_v<T>)
      m_data[src].~T();
  }

  u64 pos = (m_head + index) & m_mask;
  new (m_data + pos) T(std::move(value));
  m_tail = (m_tail + 1) & m_mask;
  ++m_size;
}

// ============================================================================
// erase — remove at logical index, shift tail left
// ============================================================================

template <typename T> void RingBuffer<T>::erase(u64 index) {
  SV_ASSERT(m_size > 0, "RingBuffer erase called on empty buffer");
  SV_ASSERT(index < m_size, "RingBuffer erase index {} out of bounds (size={})",
            index, m_size);

  u64 pos = (m_head + index) & m_mask;
  if constexpr (!std::is_trivially_destructible_v<T>)
    m_data[pos].~T();

  // shift [index+1 .. size-1] left by one
  for (u64 i = index + 1; i < m_size; ++i) {
    u64 dst = (m_head + i - 1) & m_mask;
    u64 src = (m_head + i) & m_mask;
    new (m_data + dst) T(std::move_if_noexcept(m_data[src]));
    if constexpr (!std::is_trivially_destructible_v<T>)
      m_data[src].~T();
  }

  m_tail = (m_tail - 1) & m_mask;
  --m_size;
}

// ============================================================================
// clear — destroy all elements, reset head/tail
// ============================================================================

template <typename T> void RingBuffer<T>::clear() {
  destroy_elements();
  m_size = 0;
  m_head = 0;
  m_tail = 0;
}
