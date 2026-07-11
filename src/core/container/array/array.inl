#pragma once

#include "array.hpp"

#include "core/memory/allocator.h"
#include "debug/debugger.h"
#include "platform/memory.h"

#include <algorithm>
#include <utility>

template <typename T>
Array<T>::Array(Allocator* a) : m_allocator(a) {
    m_data = nullptr;
    m_size = 0;
    m_capacity = 0;
}

template <typename T>
Array<T>::Array(u64 size, const T& value, Allocator* a)
    : m_allocator(a), m_size(size) {
    m_capacity = m_size + (m_size >> 1);  // 1.5x, integer
    if (a) {
        m_data = static_cast<T*>(a->allocate(m_capacity * sizeof(T)));
    } else {
        m_data = static_cast<T*>(
            platform_aligned_alloc(m_capacity * sizeof(T), ALIGNMENT));
    }
    for (u64 i = 0; i < m_size; i++) {
        new (m_data + i) T(value);
    }
}

template <typename T>
Array<T>::Array(Array<T>&& other) noexcept
    : m_allocator(other.m_allocator), m_data(other.m_data),
      m_size(other.m_size), m_capacity(other.m_capacity) {
    other.m_allocator = nullptr;
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template <typename T>
Array<T>& Array<T>::operator=(Array<T>&& other) noexcept {
    if (this != &other) {
        for (u64 i = 0; i < m_size; i++)
            m_data[i].~T();

        if (m_allocator)
            m_allocator->deallocate(m_data);
        else
            platform_aligned_free(m_data);

        m_allocator = other.m_allocator;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_allocator = nullptr;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    return *this;
}

template <typename T>
Array<T>::~Array() {
    if (!m_data) {
        return;
    }
    for (u64 i = 0; i < m_size; i++) {
        m_data[i].~T();
    }

    if (m_allocator)
        m_allocator->deallocate(m_data);
    else
        platform_aligned_free(m_data);

    m_size = 0;
    m_capacity = 0;
}

template <typename T>
void Array<T>::reserve(u64 newCapacity) {
    if (newCapacity <= m_capacity) {
        return;
    }

    T* newData = nullptr;
    if (m_allocator) {
        newData =
            static_cast<T*>(m_allocator->allocate(newCapacity * sizeof(T)));
    } else {
        newData = static_cast<T*>(
            platform_aligned_alloc(newCapacity * sizeof(T), ALIGNMENT));
    }

    for (u64 i = 0; i < m_size; i++) {
        new (newData + i) T(std::move_if_noexcept(m_data[i]));
    }

    if constexpr(!std::is_trivially_destructible_v<T>) {
        for (u64 i = 0; i < m_size; i++) {
            m_data[i].~T();
        }
    }

    if (m_allocator) {
        m_allocator->deallocate(m_data);
    } else {
        platform_aligned_free(m_data);
    }

    m_capacity = newCapacity;
    m_data = newData;
}

template <typename T>
void Array<T>::resize(u64 newSize, const T& value) {
    if (newSize <= m_capacity) {
        if (newSize > m_size) {
            for (u64 i = m_size; i < newSize; i++)
                new (m_data + i) T(value);
        } else {
            if constexpr(!std::is_trivially_destructible_v<T>) {
                for (u64 i = newSize; i < m_size; i++) m_data[i].~T();
            }
        }
        m_size = newSize;
        return;
    }

    if (m_size == newSize) {
        return;
    }

    T* newData = nullptr;
    if (m_allocator) {
        newData = static_cast<T*>(m_allocator->allocate(newSize * sizeof(T)));
    } else {
        newData = static_cast<T*>(
            platform_aligned_alloc(newSize * sizeof(T), ALIGNMENT));
    }

    u64 copyCount = std::min(m_size, newSize);
    for (u64 i = 0; i < copyCount; i++) {
        new (newData + i) T(std::move_if_noexcept(m_data[i]));
    }
    for (u64 i = copyCount; i < newSize; i++) {
        new (newData + i) T(value);
    }

    if constexpr(!std::is_trivially_destructible_v<T>) {
        for (u64 i = newSize; i < m_size; i++) {
            m_data[i].~T();
        }
    }

    if (m_allocator) {
        m_allocator->deallocate(m_data);
    } else {
        platform_aligned_free(m_data);
    }

    m_capacity = newSize + (newSize >> 1);  // 1.5x, integer
    m_size = newSize;
    m_data = newData;
}

template <typename T>
void Array<T>::shrink_to_fit() {}

template <typename T>
Allocator* Array<T>::allocator() const {
    return m_allocator;
}

template <typename T>
u64 Array<T>::size() const { return m_size; }

template <typename T>
u64 Array<T>::capacity() const { return m_capacity; }

template <typename T>
b32 Array<T>::empty() const { return m_size == 0; }

template <typename T>
T& Array<T>::operator[](u64 i) {
    if (i >= m_size) {
        FATAL("Index {} out of bound", i);
    }
    return m_data[i];
}

template <typename T>
const T& Array<T>::operator[](u64 i) const {
    if (i >= m_size) {
        FATAL("Index {} out of bound", i);
    }
    return m_data[i];
}

template <typename T>
T& Array<T>::front() {
    if (m_size == 0) {
        FATAL("Array is empty");
    }
    return m_data[0];
}

template <typename T>
const T& Array<T>::front() const {
    if (m_size == 0) {
        FATAL("Array is empty");
    }
    return m_data[0];
}

template <typename T>
T& Array<T>::back() {
    if (m_size == 0) {
        FATAL("Array is empty");
    }
    return m_data[m_size - 1];
}

template <typename T>
const T& Array<T>::back() const {
    if (m_size == 0) {
        FATAL("Array is empty");
    }
    return m_data[m_size - 1];
}

template <typename T>
T* Array<T>::data() { return m_data; }

template <typename T>
const T* Array<T>::data() const { return m_data; }

template <typename T>
void Array<T>::push_back(const T& value) {
    if (m_size == m_capacity) {
        u64 newCap = m_capacity == 0 ? 1 : m_capacity + (m_capacity >> 1);  // 1.5x, integer
        reserve(newCap);
    }
    new (m_data + m_size) T(value);
    m_size++;
}

template <typename T>
void Array<T>::push_back(T&& value) {
    if (m_size == m_capacity) {
        u64 newCap = m_capacity == 0 ? 1 : m_capacity + (m_capacity >> 1);
        reserve(newCap);
    }
    new (m_data + m_size) T(std::move(value));
    m_size++;
}

template <typename T>
template <typename... Args>
T& Array<T>::emplace_back(Args&&... args) {
    if (m_size == m_capacity) {
        u64 newCap = m_capacity == 0 ? 1 : m_capacity + (m_capacity >> 1);
        reserve(newCap);
    }

    T* obj = new (m_data + m_size) T(std::forward<Args>(args)...);
    ++m_size;
    return obj;
}

template <typename T>
void Array<T>::pop_back() {
    if (m_size == 0) {
        FATAL("Array is empty");
        return;
    }
    if constexpr(!std::is_trivially_destructible_v<T>) {
        m_data[m_size - 1].~T();
    }
    m_size--;
}

template <typename T>
void Array<T>::insert(u64 index, const T& value) {
    if (index > m_size) {
        FATAL("Index {} out of bound", index);
        return;
    }

    if (m_size == m_capacity) {
        u64 newCap = m_capacity == 0 ? 1 : m_capacity + (m_capacity >> 1);
        reserve(newCap);
    }

    if constexpr (std::is_trivially_copyable_v<T>) {
        u64 tail = m_size - index;
        if (tail) memmove(m_data + index + 1, m_data + index, tail * sizeof(T));
    } else {
        for (u64 i = m_size; i > index; i--) {
            new (m_data + i) T(std::move(m_data[i - 1]));
            m_data[i - 1].~T();
        }
    }

    new (m_data + index) T(value);
    m_size++;
}

template <typename T>
void Array<T>::insert(u64 index, T&& value) {
    if (index > m_size) {
        FATAL("Index {} out of bound", index);
        return;
    }

    if (m_size == m_capacity) {
        u64 newCap = m_capacity == 0 ? 1 : m_capacity + (m_capacity >> 1);
        reserve(newCap);
    }

    if constexpr (std::is_trivially_copyable_v<T>) {
        u64 tail = m_size - index;
        if (tail) memmove(m_data + index + 1, m_data + index, tail * sizeof(T));
    } else {
        for (u64 i = m_size; i > index; i--) {
            new (m_data + i) T(std::move(m_data[i - 1]));
            m_data[i - 1].~T();
        }
    }

    new (m_data + index) T(std::move(value));
    m_size++;
}

template <typename T>
void Array<T>::clear() {
    if (m_size == 0) {
        return;
    }
    if constexpr(!std::is_trivially_destructible_v<T>) {
        for (u64 i = 0; i < m_size; i++) {
            m_data[i].~T();
        }
    }
    m_size = 0;
}

template <typename T>
void Array<T>::erase(u64 index) {
    if (m_size == 0) {
        FATAL("Array is empty");
        return;
    }
    if (index >= m_size) {
        FATAL("Index {} out of bound", index);
        return;
    }

    if constexpr (std::is_trivially_copyable_v<T>) {
        u64 tail = m_size - index - 1;
        if (tail) memmove(m_data + index, m_data + index + 1, tail * sizeof(T));
    } else {
        m_data[index].~T();
        for (u64 i = index; i < m_size - 1; i++) {
            new (m_data + i) T(std::move(m_data[i + 1]));
            m_data[i + 1].~T();
        }
    }

    m_size--;
}
