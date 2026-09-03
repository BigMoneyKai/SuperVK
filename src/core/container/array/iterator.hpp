#pragma once

#include "core/define/types.h"

template <typename T> class ArrayIterator {
public:
  using Val_T = T;
  using Ptr = T *;
  using Ref = T &;

  ArrayIterator() = default;
  explicit ArrayIterator(Ptr ptr) : m_ptr(ptr) {}

  Ref operator*() const { return *m_ptr; }
  Ptr operator->() const { return m_ptr; }

  ArrayIterator &operator++() {
    ++m_ptr;
    return *this;
  }
  ArrayIterator operator++(i32) {
    ArrayIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  ArrayIterator &operator--() {
    --m_ptr;
    return *this;
  }
  ArrayIterator operator--(i32) {
    ArrayIterator tmp = *this;
    --(*this);
    return tmp;
  }

  ArrayIterator operator+(i64 offset) const {
    return ArrayIterator(m_ptr + offset);
  }
  ArrayIterator operator-(i64 offset) const {
    return ArrayIterator(m_ptr - offset);
  }

  i64 operator-(const ArrayIterator &rhs) const { return m_ptr - rhs.m_ptr; }

  Ref operator[](i64 index) const { return m_ptr[index]; }

  ArrayIterator &operator+=(i64 offset) {
    m_ptr += offset;
    return *this;
  }
  ArrayIterator &operator-=(i64 offset) {
    m_ptr -= offset;
    return *this;
  }

  b32 operator==(const ArrayIterator &rhs) const { return m_ptr == rhs.m_ptr; }
  b32 operator!=(const ArrayIterator &rhs) const { return m_ptr != rhs.m_ptr; }
  b32 operator<(const ArrayIterator &rhs) const { return m_ptr < rhs.m_ptr; }
  b32 operator>(const ArrayIterator &rhs) const { return m_ptr > rhs.m_ptr; }
  b32 operator<=(const ArrayIterator &rhs) const { return m_ptr <= rhs.m_ptr; }
  b32 operator>=(const ArrayIterator &rhs) const { return m_ptr >= rhs.m_ptr; }

private:
  Ptr m_ptr{nullptr};
};
