#pragma once

#include "iterator.hpp"
#include "reverse_iterator.hpp"
#include "core/define/compiler.h"
#include "core/memory/allocator.h"

template <typename T> class Array {
public:
  using iterator = ArrayIterator<T>;
  using const_iterator = ArrayIterator<const T>;
  using reverse_iterator = ArrayReverseIterator<iterator>;
  using const_reverse_iterator = ArrayReverseIterator<const_iterator>;

  explicit Array(Allocator *a = &default_allocator());
  Array(u64 capacity, const T &value = T{},
        Allocator *a = &default_allocator());
  Array(void *data, u64 size, Allocator *a = &default_allocator());

  // copy not allowed, move allowed
  Array(const Array &) = delete;
  Array &operator=(const Array &) = delete;
  Array(Array &&other) noexcept;
  Array &operator=(Array &&other) noexcept;

  ~Array();

  void reserve(u64 capacity);
  void resize(u64 size, const T &value = T{});
  Allocator *allocator() const;
  u64 size() const;
  u64 capacity() const;
  b32 empty() const;

  SV_FORCE_INLINE T& operator[](u64 i) {
    SV_ASSERT(i < m_size, "Index {} out of bound", i);
    return m_data[i];
  }
  SV_FORCE_INLINE const T& operator[](u64 i) const {
    SV_ASSERT(i < m_size, "Index {} out of bound", i);
    return m_data[i];
  }
  T &at(u64 i);
  const T &at(u64 i) const;
  T &front();
  const T &front() const;
  T &back();
  const T &back() const;
  T *data();
  const T *data() const;

  void push_back(const T &value);
  void push_back(T &&value);
  template <typename... Args> T &emplace_back(Args &&...args);
  void pop_back();
  void insert(u64 index, const T &value);
  void insert(u64 index, T &&value);
  void append(const T *data, u64 size);
  void erase(u64 index);
  void clear();

  SV_FORCE_INLINE iterator begin() { return iterator(m_data); }
  SV_FORCE_INLINE const_iterator begin() const {
    return const_iterator(m_data);
  }
  SV_FORCE_INLINE iterator end() {
    return iterator(m_data + static_cast<addr>(m_size));
  }
  SV_FORCE_INLINE const_iterator end() const {
    return const_iterator(m_data + static_cast<addr>(m_size));
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
  Allocator *m_allocator{nullptr};
  T *m_data{nullptr};
  u64 m_size{0};
  u64 m_capacity{0};

  static constexpr f32 GROWTH_FACTOR = 1.5f;
  static constexpr u64 ALIGNMENT = 16;
};

#include "array.inl"
