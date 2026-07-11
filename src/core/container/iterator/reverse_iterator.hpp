#pragma once

#include "defines.h"

template <typename It>
class ReverseIterator {
  public:
    explicit ReverseIterator(It it)
        : m_it(it) {}

    auto& operator*() const {
        It tmp = m_it;
        --tmp;
        return *tmp;
    }

    auto& operator->() const {
        It tmp = m_it;
        --tmp;
        return tmp.operator->();
    }

    ReverseIterator& operator++() {
        --m_it;
        return *this;
    }
    ReverseIterator operator++(i32) {
        ReverseIterator tmp = *this;
        --m_it;
        return tmp;
    }

    ReverseIterator& operator--() {
        ++m_it;
        return *this;
    }
    ReverseIterator operator--(i32) {
        ReverseIterator tmp = *this;
        ++m_it;
        return tmp;
    }

    b32 operator==(const ReverseIterator& rhs) const {
        return m_it == rhs.m_it;
    }
    b32 operator!=(const ReverseIterator& rhs) const {
        return m_it != rhs.m_it;
    }

  private:
    It m_it;
};
