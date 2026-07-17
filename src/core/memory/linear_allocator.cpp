#include "linear_allocator.h"
#include "platform/memory.h"
#include "utils/utils.h"
#include "allocator_type.h"
#include "alloc_counter.h"

LinearAllocator::LinearAllocator(u64 size, Allocator* backing) {
    m_size = size;
    m_totalSize = size;
    m_cursor = 0;
    m_backing = backing;

    if(backing) {
        m_buffer = static_cast<u8*>(backing->allocate(size, alignof(std::max_align_t)));
    } else {
        m_buffer = static_cast<u8*>(platform_aligned_alloc(size, alignof(std::max_align_t)));
    }

    SV_ASSERT(m_buffer, "Allocator buffer not initialized");
}

LinearAllocator::~LinearAllocator() {
    if(!m_buffer) {
        return;
    }

    if(m_backing) {
        m_backing->deallocate(m_buffer, m_size);
#ifndef NDEBUG
        m_totalSize = 0;
        m_usedSize = 0;
        m_freeSize = 0;
#endif
    } else {
        platform_aligned_free(m_buffer);
    }
}

void* LinearAllocator::allocate(u64 size, u64 alignment) {
#ifndef NDEBUG
    constexpr u64 headerAlignment = alignof(Header);
    u64 headerSize = align_up(sizeof(Header), headerAlignment);

    u64 headerOffset = align_up(m_cursor, headerAlignment);
    u64 userOffset = align_up(headerOffset + headerSize, alignment);

    if(userOffset + size > m_size) {
        return nullptr;
    }

    Header* header = reinterpret_cast<Header*>(static_cast<u8*>(m_buffer) + headerOffset);
    header->size = size;
    header->alignment = alignment;
    header->allocationId = AllocCounter::add();
    header->allocatorId = AllocatorType::Linear;
    header->state = HeaderState::Allocated;
    header->magic = HeaderState::Magic;

    m_usedSize += size;
    m_freeSize -= size;

    void* ptr = static_cast<u8*>(m_buffer) + userOffset;
    m_cursor = userOffset + size;
    return ptr;
#else
    u64 offset = align_up(m_cursor, alignment);

    if(offset + size > m_size) {
        return nullptr;
    }

    void* ptr = static_cast<u8*>(m_buffer) + offset;
    m_cursor = offset + size;
    return ptr;
#endif
}

void LinearAllocator::deallocate(void* ptr, u64 size) {
    (void)ptr;
    (void)size;
    return;
}

void LinearAllocator::reset() {
    m_cursor = 0;

#ifndef NDEBUG
    m_usedSize = 0;
    m_freeSize = m_totalSize;
#endif
}
