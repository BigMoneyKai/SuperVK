#include "allocator.h"
#include "heap_allocator.h"

Allocator& default_allocator() {
    return HeapAllocator::instance();
}

u64 Allocator::allocated_size(void*) {
    return 0;
}

u64 Allocator::total_allocated() const {
    return 0;
}

u64 Allocator::total_freed() const {
    return 0;
}

