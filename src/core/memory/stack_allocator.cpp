#include "stack_allocator.h"
#include "platform/memory.h"
#include "utils/utils.h"

StackAllocator::StackAllocator(u64 size, Allocator* backing) {
    m_size = size;
    m_cursor = 0;
    m_backing = backing;

    if(backing) {
        m_buffer = static_cast<u8*>(backing->allocate(size, alignof(std::max_align_t)));
    } else {
        m_buffer = static_cast<u8*>(platform_aligned_alloc(size, alignof(std::max_align_t)));
    }
}

StackAllocator::~StackAllocator() {
    if(!m_buffer) {
        return;
    }

    if(m_backing) {
        m_backing->deallocate(m_buffer);
    } else {
        platform_aligned_free(m_buffer);
    }
}

void* StackAllocator::allocate(u64 size, u64 alignment) {
    u64 offset = align_up(m_cursor, alignment);

    if(offset + size > m_size) {
        return nullptr;
    }

    void* ptr = static_cast<u8*>(m_buffer) + offset;
    m_cursor = offset + size;
    return ptr;
}

void StackAllocator::reset_to(Marker marker) {
    m_cursor = marker.cursor;
}

void StackAllocator::reset() {
    m_cursor = 0;
}
