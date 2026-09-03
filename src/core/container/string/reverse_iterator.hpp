#pragma once

#include "core/define/types.h"

template <typename It> class StringReverseIterator {
public:
  explicit StringReverseIterator(It it) : m_it(it) {}

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

  StringReverseIterator &operator++() {
    --m_it;
    return *this;
  }
  StringReverseIterator operator++(i32) {
    StringReverseIterator tmp = *this;
    --m_it;
    return tmp;
  }

  StringReverseIterator &operator--() {
    ++m_it;
    return *this;
  }
  StringReverseIterator operator--(i32) {
    StringReverseIterator tmp = *this;
    ++m_it;
    return tmp;
  }

  b32 operator==(const StringReverseIterator &rhs) const { return m_it == rhs.m_it; }
  b32 operator!=(const StringReverseIterator &rhs) const { return m_it != rhs.m_it; }

private:
  It m_it;
};
