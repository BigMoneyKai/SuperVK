#pragma once

#include "core/define/types.h"

template <typename It> class ArrayReverseIterator {
public:
  explicit ArrayReverseIterator(It it) : m_it(it) {}

  auto &operator*() const {
    It tmp = m_it;
    --tmp;
    return *tmp;
  }

  auto &operator->() const {
    It tmp = m_it;
    --tmp;
    return tmp.operator->();
  }

  ArrayReverseIterator &operator++() {
    --m_it;
    return *this;
  }
  ArrayReverseIterator operator++(i32) {
    ArrayReverseIterator tmp = *this;
    --m_it;
    return tmp;
  }

  ArrayReverseIterator &operator--() {
    ++m_it;
    return *this;
  }
  ArrayReverseIterator operator--(i32) {
    ArrayReverseIterator tmp = *this;
    ++m_it;
    return tmp;
  }

  b32 operator==(const ArrayReverseIterator &rhs) const { return m_it == rhs.m_it; }
  b32 operator!=(const ArrayReverseIterator &rhs) const { return m_it != rhs.m_it; }

private:
  It m_it;
};
