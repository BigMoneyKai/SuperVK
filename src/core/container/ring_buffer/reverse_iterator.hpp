#pragma once

#include "core/define/types.h"

template <typename It> class RingBufferReverseIterator {
public:
  RingBufferReverseIterator() = default;
  RingBufferReverseIterator(It it)
      : m_it(it) {}

  auto operator*() const { 
    It tmp = m_it;
    --tmp;
    return *m_it; 
  }
  auto operator->() const { 
    It tmp = m_it;
    --tmp;
    return m_it.operator->(); 
  }

  RingBufferReverseIterator &operator++() {
    --m_it;
    return *this;
  }

  RingBufferReverseIterator operator++(int) {
    RingBufferReverseIterator tmp = *this;
    --m_it;
    return tmp;
  }

  RingBufferReverseIterator &operator--() {
    ++m_it;
    return *this;
  }
  RingBufferReverseIterator operator--(int) {
    RingBufferReverseIterator tmp = *this;
    ++m_it;
    return tmp;
  }

  b32 operator==(const RingBufferReverseIterator &rhs) const {
    return m_it == rhs.m_it;
  }
  b32 operator!=(const RingBufferReverseIterator &rhs) const {
    return m_it != rhs.m_it;
  }

private:
  It m_it;
};
