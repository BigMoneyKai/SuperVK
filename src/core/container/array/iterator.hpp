#pragma once

#include "core/define/types.h"

template <typename T> class Iterator {
public:
  using Val_T = T;
  using Ptr = T *;
  using Ref = T &;

  Iterator() = default;
  explicit Iterator(Ptr ptr) : m_ptr(ptr) {}

  Ref operator*() const { return *m_ptr; }
  Ptr operator->() const { return m_ptr; }

  Iterator &operator++() {
    ++m_ptr;
    return *this;
  }
  Iterator operator++(i32) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  Iterator &operator--() {
    --m_ptr;
    return *this;
  }
  Iterator operator--(i32) {
    Iterator tmp = *this;
    --(*this);
    return tmp;
  }

  Iterator operator+(i64 offset) const {
    return Iterator(m_ptr + offset);
  }
  Iterator operator-(i64 offset) const {
    return Iterator(m_ptr - offset);
  }

  i64 operator-(const Iterator &rhs) const { return m_ptr - rhs.m_ptr; }

  Ref operator[](i64 index) const { return m_ptr[index]; }

  Iterator &operator+=(i64 offset) {
    m_ptr += offset;
    return *this;
  }
  Iterator &operator-=(i64 offset) {
    m_ptr -= offset;
    return *this;
  }

  b32 operator==(const Iterator &rhs) const { return m_ptr == rhs.m_ptr; }
  b32 operator!=(const Iterator &rhs) const { return m_ptr != rhs.m_ptr; }
  b32 operator<(const Iterator &rhs) const { return m_ptr < rhs.m_ptr; }
  b32 operator>(const Iterator &rhs) const { return m_ptr > rhs.m_ptr; }
  b32 operator<=(const Iterator &rhs) const { return m_ptr <= rhs.m_ptr; }
  b32 operator>=(const Iterator &rhs) const { return m_ptr >= rhs.m_ptr; }

private:
  Ptr m_ptr{nullptr};
};
