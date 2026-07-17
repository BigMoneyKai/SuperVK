#include "allocator.h"
#include "alloc_counter.h"
#include "heap_allocator.h"
#include "utils/utils.h"

u64 AllocCounter::m_count = 0;

Allocator& default_allocator() {
    return HeapAllocator::instance();
}

#ifndef NDEBUG
u64 Allocator::totalSize() const {
    return m_totalSize;
}

u64 Allocator::allocSize(void* ptr) const {
    constexpr u64 headerAlignment = alignof(Header);
    u64 headerSize = align_up(sizeof(Header), headerAlignment);
    Header* headerAddr = reinterpret_cast<Header*>(static_cast<u8*>(ptr) - headerSize);
    return headerAddr->size;
}

u64 Allocator::usedSize() const {
    return m_usedSize;
}

u64 Allocator::freeSize() const {
    return m_freeSize;
}
#else
u64 Allocator::totalSize() const {
    return 0;
}

u64 Allocator::allocSize(void* ptr) const {
    return 0;
}

u64 Allocator::usedSize() const {
    return 0;
}

u64 Allocator::freeSize() const {
    return 0;
}
#endif
