#pragma once

#include "allocator.h"

class LinearAllocator : public Allocator {
public:
    LinearAllocator(u64 size, Allocator* backing = &default_allocator());
    ~LinearAllocator();

    void* allocate(u64 size, u64 alignment = 16) override;

    void reset();

    SV_FORCE_INLINE u64 used() const {
        return m_cursor;
    }

    SV_FORCE_INLINE u64 capacity() {
        return m_size;
    }
    SV_FORCE_INLINE u64 remaining() {
        return m_size - m_cursor;
    }

private:
    void* m_buffer{nullptr};
    u64 m_size;
    u64 m_cursor;
    Allocator* m_backing{nullptr};
};
