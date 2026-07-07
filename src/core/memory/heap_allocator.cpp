#include "heap_allocator.h"
#include "platform/memory.h"

HeapAllocator& HeapAllocator::instance() {
    static HeapAllocator s_instance;
    return s_instance;
}

void* HeapAllocator::allocate(u64 size, u64 alignment) {
    void* ptr = platform_aligned_alloc(size, alignment);

#ifndef NDEBUG
    if (ptr) {
        m_totalAllocated += size;
    }
#endif
    return ptr;
}

void HeapAllocator::deallocate(void* ptr) {
    if (!ptr) return;
    platform_aligned_free(ptr);
}

u64 HeapAllocator::total_allocated() const {
#ifndef NDEBUG
    return m_totalAllocated;
#else
    return 0;
#endif
}
