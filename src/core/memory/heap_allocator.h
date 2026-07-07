#pragma once

#include "allocator.h"

class HeapAllocator : public Allocator {
public:
    void* allocate(u64 size,   u64 alignment = 16) override;
    void  deallocate(void* ptr) override;
    u64   total_allocated()  const override;

    static HeapAllocator& instance();

    HeapAllocator(const HeapAllocator&) = delete;
    HeapAllocator& operator=(const HeapAllocator&) = delete;

private:
    HeapAllocator() = default;

#ifndef NDEBUG
    u64 m_totalAllocated = 0;
#endif
};
