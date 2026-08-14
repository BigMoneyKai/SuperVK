#pragma once

#include "core/define/types.h"

template <typename T> class RingBufferIterator {
public:
  using Val_T = T;
  using Ptr = T *;
  using Ref = T &;

  RingBufferIterator() = default;

  RingBufferIterator(Ptr data, u64 head, u64 mask, u64 offset)
      : m_data(data), m_head(head), m_mask(mask), m_offset(offset) {}

  Ref operator*() const { return m_data[(m_head + m_offset) & m_mask]; }
  Ptr operator->() const { return &m_data[(m_head + m_offset) & m_mask]; }

  RingBufferIterator &operator++() {
    ++m_offset;
    return *this;
  }
  RingBufferIterator operator++(int) {
    RingBufferIterator tmp = *this;
    ++m_offset;
    return tmp;
  }

  RingBufferIterator &operator--() {
    --m_offset;
    return *this;
  }
  RingBufferIterator operator--(int) {
    RingBufferIterator tmp = *this;
    --m_offset;
    return tmp;
  }

  bool operator==(const RingBufferIterator &rhs) const {
    return m_offset == rhs.m_offset;
  }
  bool operator!=(const RingBufferIterator &rhs) const {
    return m_offset != rhs.m_offset;
  }

private:
  Ptr m_data{nullptr};
  u64 m_head{0};
  u64 m_mask{0};
  u64 m_offset{0}; // logical position: 0 = front, m_size = end
};
