#include "heap_allocator.h"
#include "defines.h"
#include "platform/memory.h"
#include "utils/utils.h"
#include "allocator_type.h"
#include "alloc_counter.h"

#ifndef NDEBUG
HeapAllocator& HeapAllocator::instance() {
    static HeapAllocator s_instance;
    return s_instance;
}

void* HeapAllocator::allocate(u64 size, u64 alignment) {
    constexpr u64 headerAlignment = alignof(Header);
    u64 headerSize = align_up(sizeof(Header), headerAlignment);
    u64 allocAlignment = alignment > headerAlignment ? alignment : headerAlignment;

    u8* realPtr = static_cast<u8*>(platform_aligned_alloc(size + headerSize, allocAlignment));
    if(!realPtr) {
        return realPtr;
    }
    u8* userPtr = realPtr + headerSize;
    Header* header = reinterpret_cast<Header*>(realPtr);
    header->size = size;
    header->alignment = alignment;
    header->allocationId = AllocCounter::add();
    header->allocatorId = AllocatorType::Heap;
    header->state = HeaderState::Allocated;
    header->magic = HeaderState::Magic;

    m_usedSize += size;
    m_freeSize -= size;
    return userPtr;
}

void HeapAllocator::deallocate(void* ptr, u64 size) {
    if (!ptr) {
        return;
    }
    constexpr u64 HeaderAlignment = alignof(Header);
    const u64 headerSize = align_up(sizeof(Header), HeaderAlignment);
    u8* realPtr = reinterpret_cast<u8*>(static_cast<u8*>(ptr) - headerSize);
    Header* header = reinterpret_cast<Header*>(realPtr);

    header->state = HeaderState::Freed;

    m_usedSize -= size;
    m_freeSize += size;
    platform_aligned_free(realPtr);
}
#else 
HeapAllocator& HeapAllocator::instance() {
    static HeapAllocator s_instance;
    return s_instance;
}

void* HeapAllocator::allocate(u64 size, u64 alignment) {
    void* ptr = platform_aligned_alloc(size, alignment);

    return ptr;
}

void HeapAllocator::deallocate(void* ptr, u64 size) {
    if (!ptr) {
        return;
    }

    platform_aligned_free(ptr);
}
#endif