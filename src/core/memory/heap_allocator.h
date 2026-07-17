#pragma once

#include "allocator.h"

class HeapAllocator : public Allocator {
public:
    void* allocate(u64 size, u64 alignment = DEFAULT_ALIGNMENT) override;
    void  deallocate(void* ptr, u64 size) override;

    static HeapAllocator& instance();

    HeapAllocator(const HeapAllocator&) = delete;
    HeapAllocator& operator=(const HeapAllocator&) = delete;

private:
    HeapAllocator() = default;

};
