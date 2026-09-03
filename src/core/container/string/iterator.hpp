#pragma once

#include "core/define/types.h"

template <typename T> class StringIterator {
public:
  using Val_T = T;
  using Ptr = T *;
  using Ref = T &;

  StringIterator() = default;
  explicit StringIterator(Ptr ptr) : m_ptr(ptr) {}

  Ref operator*() const { return *m_ptr; }
  Ptr operator->() const { return m_ptr; }

  StringIterator &operator++() {
    ++m_ptr;
    return *this;
  }
  StringIterator operator++(i32) {
    StringIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  StringIterator &operator--() {
    --m_ptr;
    return *this;
  }
  StringIterator operator--(i32) {
    StringIterator tmp = *this;
    --(*this);
    return tmp;
  }

  StringIterator operator+(i64 offset) const {
    return StringIterator(m_ptr + offset);
  }
  StringIterator operator-(i64 offset) const {
    return StringIterator(m_ptr - offset);
  }

  i64 operator-(const StringIterator &rhs) const { return m_ptr - rhs.m_ptr; }

  Ref operator[](i64 index) const { return m_ptr[index]; }

  StringIterator &operator+=(i64 offset) {
    m_ptr += offset;
    return *this;
  }
  StringIterator &operator-=(i64 offset) {
    m_ptr -= offset;
    return *this;
  }

  b32 operator==(const StringIterator &rhs) const { return m_ptr == rhs.m_ptr; }
  b32 operator!=(const StringIterator &rhs) const { return m_ptr != rhs.m_ptr; }
  b32 operator<(const StringIterator &rhs) const { return m_ptr < rhs.m_ptr; }
  b32 operator>(const StringIterator &rhs) const { return m_ptr > rhs.m_ptr; }
  b32 operator<=(const StringIterator &rhs) const { return m_ptr <= rhs.m_ptr; }
  b32 operator>=(const StringIterator &rhs) const { return m_ptr >= rhs.m_ptr; }

private:
  Ptr m_ptr{nullptr};
};
