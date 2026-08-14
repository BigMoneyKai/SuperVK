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

  virtual Iterator &operator++() {
    ++m_ptr;
    return *this;
  }
  virtual Iterator operator++(i32) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  virtual Iterator &operator--() {
    --m_ptr;
    return *this;
  }
  virtual Iterator operator--(i32) {
    Iterator tmp = *this;
    --(*this);
    return tmp;
  }

  virtual Iterator operator+(i64 offset) const {
    return Iterator(m_ptr + offset);
  }
  virtual Iterator operator-(i64 offset) const {
    return Iterator(m_ptr - offset);
  }

  virtual i64 operator-(const Iterator &rhs) const { return m_ptr - rhs.m_ptr; }

  Ref operator[](i64 index) const { return m_ptr[index]; }

  virtual Iterator &operator+=(i64 offset) {
    m_ptr += offset;
    return *this;
  }
  virtual Iterator &operator-=(i64 offset) {
    m_ptr -= offset;
    return *this;
  }

  b32 operator==(const Iterator &rhs) const { return m_ptr == rhs.m_ptr; }
  b32 operator!=(const Iterator &rhs) const { return m_ptr != rhs.m_ptr; }
  b32 operator<(const Iterator &rhs) const { return m_ptr < rhs.m_ptr; }
  b32 operator>(const Iterator &rhs) const { return m_ptr > rhs.m_ptr; }
  b32 operator<=(const Iterator &rhs) const { return m_ptr <= rhs.m_ptr; }
  b32 operator>=(const Iterator &rhs) const { return m_ptr >= rhs.m_ptr; }

protected:
  Ptr m_ptr{nullptr};
};
