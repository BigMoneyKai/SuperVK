#pragma once

#include "defines.h"

template <typename T>
class Iterator {
public:
    using Value_type = T;
    using Ptr = T*;
    using Ref = T&;

    Iterator() = default;
    explicit Iterator(Ptr ptr)
    : m_ptr(ptr) {}

    Ref operator*() const {
        return *m_ptr;
    }
    Ptr operator->() const {
        return m_ptr;
    }

    Iterator& operator++() {
        ++m_ptr;
        return *this;
    }

    Iterator operator++(i32) {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    Iterator& operator--() {
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

    b32 operator==(const Iterator& rhs) const {
        return m_ptr == rhs.m_ptr;
    }

    b32 operator!=(const Iterator& rhs) const {
        return m_ptr != rhs.m_ptr;
    }

private:
    Ptr m_ptr;
};
