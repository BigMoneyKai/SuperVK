#pragma once

#include "defines.h"
#include <cstddef>

class Allocator {
public:
    virtual ~Allocator() = default;

    virtual void* allocate(u64 size, u64 alignment = 16) = 0;
    virtual void  deallocate(void* ptr) = 0;

    virtual u64 allocated_size(void* ptr);
    virtual u64 total_allocated() const;
    virtual u64 total_freed() const;
};

// Default global allocator
Allocator& default_allocator();
