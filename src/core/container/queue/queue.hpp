#pragma once

#include "defines.h"
#include "core/memory/allocator.h"
#include "core/container/iterator/iterator.hpp"
#include "core/container/iterator/reverse_iterator.hpp"

template <typename T>
class Queue {
  public:
    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    using reverse_iterator = ReverseIterator<iterator>;
    using const_reverse_iterator = ReverseIterator<const_iterator>;

    explicit Queue(Allocator* a = &default_allocator());
    Queue(u64 capacity, const T& value = T{}, Allocator* a = &default_allocator());

    // copy not allowed, move allowed
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
    Queue(Queue&& other) noexcept;
    Queue& operator=(Queue&& other) noexcept;

    ~Queue();

    void reserve(u64 capacity);
    void resize(u64 size, const T& value = T{});
    void shrink_to_fit();
    Allocator* allocator() const;
    u64 size() const;
    u64 capacity() const;
    b32 empty() const;

    T& operator[](u64 i);
    const T& operator[](u64 i) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T* data();
    const T* data() const;

    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front();

    template <typename... Args>
    T& emplace_back(Args&&... args);
    void insert(u64 index, const T& value);
    void insert(u64 index, T&& value);
    void erase(u64 index);
    void clear();

    SV_FORCE_INLINE iterator begin() {
        return iterator(m_data);
    }
    SV_FORCE_INLINE const_iterator begin() const {
        return const_iterator(m_data);
    }
    SV_FORCE_INLINE iterator end() {
        return iterator(m_data + static_cast<addr>(m_size));
    }
    SV_FORCE_INLINE const_iterator end() const {
        return const_iterator(m_data + static_cast<addr>(m_size));
    }

    SV_FORCE_INLINE reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    SV_FORCE_INLINE const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    SV_FORCE_INLINE reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    SV_FORCE_INLINE const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

  private:
    Allocator* m_allocator{nullptr};
    T* m_data{nullptr};
    u64 m_size{0};
    u64 m_capacity{0};

    static constexpr f32 GROWTH_FACTOR = 1.5f;
    static constexpr u64 ALIGNMENT = 16;
};

#include "queue.inl"
