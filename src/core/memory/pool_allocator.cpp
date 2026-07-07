#include "pool_allocator.h"
#include "platform/memory.h"
#include "utils/utils.h"

PoolAllocator::PoolAllocator(u64 chunkSize, u64 chunkCount, u32 alignment, Allocator* backing)
: m_chunkSize(chunkSize), m_chunkCount(chunkCount), m_backing(backing)
{
    m_chunkSize = align_up(chunkSize, alignment);
    u64 blockSize = m_chunkSize * m_chunkCount;

    if(m_backing) {
        m_start = static_cast<u8*>(m_backing->allocate(blockSize, alignment));
    } else {
        m_start = static_cast<u8*>(platform_aligned_alloc(blockSize, alignment));
    }

    for(u64 i = 0; i < chunkCount; i++) {
        u8* chunk = static_cast<u8*>(m_start) + i * chunkSize;
        auto* node = reinterpret_cast<FreeNode*>(chunk);
        node->next = m_freeList;
        m_freeList = node;
    }
}

PoolAllocator::~PoolAllocator() {
    if(!m_start) {
        return;
    }
    m_freeList = nullptr;

    if(m_backing) {
        m_backing->deallocate(m_start);
    } else {
        platform_aligned_free(m_start);
    }
}

void* PoolAllocator::allocate(u64 size, u64 alignment) {
    if(size > m_chunkSize || alignment > m_chunkSize) {
        return nullptr;
    }
    if(!m_freeList) {
        return nullptr;
    }

    FreeNode* node = m_freeList;
    m_freeList = node->next;
    return node;
}

void PoolAllocator::deallocate(void* ptr) {
    if(!ptr) {
        return;
    }
    auto* node = reinterpret_cast<FreeNode*>(ptr);
    node->next = m_freeList;
    m_freeList = node;
    m_freeCount++;
}

