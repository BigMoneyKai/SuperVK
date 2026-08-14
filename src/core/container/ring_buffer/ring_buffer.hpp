#pragma once

#include "core/container/iterator/reverse_iterator.hpp"
#include "core/define/compiler.h"
#include "core/define/types.h"
#include "core/memory/allocator.h"
#include "ring_iterator.hpp"

template <typename T> class RingBuffer {
public:
  using iterator = RingBufferIterator<T>;
  using const_iterator = RingBufferIterator<const T>;
  using reverse_iterator = ReverseIterator<iterator>;
  using const_reverse_iterator = ReverseIterator<const_iterator>;

  explicit RingBuffer(Allocator *a = &default_allocator());
  RingBuffer(u64 capacity, const T &value = T{},
             Allocator *a = &default_allocator());

  // copy not allowed, move allowed
  RingBuffer(const RingBuffer &) = delete;
  RingBuffer &operator=(const RingBuffer &) = delete;
  RingBuffer(RingBuffer &&other) noexcept;
  RingBuffer &operator=(RingBuffer &&other) noexcept;

  ~RingBuffer();

  void reserve(u64 newCapacity);
  void resize(u64 newSize, const T &value = T{});
  void shrink_to_fit();

  Allocator *allocator() const { return m_allocator; }
  u64 size() const { return m_size; }
  u64 capacity() const { return m_capacity; }
  b32 empty() const { return m_size == 0; }
  b32 full() const { return m_size == m_capacity; }

  T &operator[](u64 i);
  const T &operator[](u64 i) const;
  T &at(u64 i);
  const T &at(u64 i) const;

  T &front() {
    SV_ASSERT(m_size > 0, "RingBuffer is empty");
    return m_data[m_head];
  }
  const T &front() const {
    SV_ASSERT(m_size > 0, "RingBuffer is empty");
    return m_data[m_head];
  }
  T &back() {
    SV_ASSERT(m_size > 0, "RingBuffer is empty");
    return m_data[(m_tail - 1) & m_mask];
  }
  const T &back() const {
    SV_ASSERT(m_size > 0, "RingBuffer is empty");
    return m_data[(m_tail - 1) & m_mask];
  }
  T *data() { return m_data; }
  const T *data() const { return m_data; }

  void push_back(const T &value);
  void push_back(T &&value);
  template <typename... Args> T &emplace_back(Args &&...args);
  void pop_front();

  void insert(u64 index, const T &value);
  void insert(u64 index, T &&value);
  void erase(u64 index);
  void clear();

  // --- iteration ---
  SV_FORCE_INLINE iterator begin() {
    return iterator(m_data, m_head, m_mask, 0);
  }
  SV_FORCE_INLINE const_iterator begin() const {
    return const_iterator(m_data, m_head, m_mask, 0);
  }
  SV_FORCE_INLINE iterator end() {
    return iterator(m_data, m_head, m_mask, m_size);
  }
  SV_FORCE_INLINE const_iterator end() const {
    return const_iterator(m_data, m_head, m_mask, m_size);
  }

  SV_FORCE_INLINE reverse_iterator rbegin() { return reverse_iterator(end()); }
  SV_FORCE_INLINE const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  SV_FORCE_INLINE reverse_iterator rend() { return reverse_iterator(begin()); }
  SV_FORCE_INLINE const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

private:
  // head → first valid element; tail → next free slot (both masked)
  Allocator *m_allocator{nullptr};
  T *m_data{nullptr};
  u64 m_size{0};
  u64 m_capacity{0};
  u64 m_mask{0};
  u64 m_head{0};
  u64 m_tail{0};

  static constexpr u64 ALIGNMENT = 16;

  SV_FORCE_INLINE static u64 next_pow2(u64 v) {
    if (v < 2)
      return 2;
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    return v + 1;
  }

  T *alloc_buffer(u64 count);
  void free_buffer(T *ptr);
  void destroy_elements();
};

#include "ring_buffer.inl"
