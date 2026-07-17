#include "pool_allocator.h"
#include "platform/memory.h"
#include "utils/utils.h"
#include "allocator_type.h"
#include "alloc_counter.h"

PoolAllocator::PoolAllocator(u64 chunkSize, u64 chunkCount, u32 alignment, Allocator* backing)
: m_chunkSize(chunkSize), m_chunkCount(chunkCount), m_backing(backing)
{
    m_chunkSize = align_up(chunkSize, alignment);

#ifndef NDEBUG
    constexpr u64 headerAlignment = alignof(Header);
    const u64 headerSize = align_up(sizeof(Header), headerAlignment);
    u64 chunkStride = m_chunkSize + headerSize;
#else
    u64 chunkStride = m_chunkSize;
#endif

    u64 blockSize = chunkStride * m_chunkCount;

    if(m_backing) {
        m_start = static_cast<u8*>(m_backing->allocate(blockSize, alignment));
    } else {
        m_start = static_cast<u8*>(platform_aligned_alloc(blockSize, alignment));
    }

    SV_ASSERT(m_start, "Allocator buffer not initialized");

    m_totalSize = blockSize;
    m_usedSize = 0;
    m_freeSize = blockSize;
    m_freeCount = m_chunkCount;

    for(u64 i = 0; i < m_chunkCount; i++) {
        u8* chunk = static_cast<u8*>(m_start) + i * chunkStride;
#ifndef NDEBUG
        u8* userData = chunk + headerSize;
        auto* node = reinterpret_cast<FreeNode*>(userData);
#else
        auto* node = reinterpret_cast<FreeNode*>(chunk);
#endif
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
        m_backing->deallocate(m_start, m_totalSize);
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
    m_freeCount--;

#ifndef NDEBUG
    constexpr u64 headerAlignment = alignof(Header);
    const u64 headerSize = align_up(sizeof(Header), headerAlignment);

    void* userPtr = node;
    Header* header = reinterpret_cast<Header*>(static_cast<u8*>(userPtr) - headerSize);
    header->size = size;
    header->alignment = alignment;
    header->allocationId = AllocCounter::add();
    header->allocatorId = AllocatorType::Pool;
    header->state = HeaderState::Allocated;
    header->magic = HeaderState::Magic;

    m_usedSize += size;
    m_freeSize -= size;
    return userPtr;
#else
    return node;
#endif
}

void PoolAllocator::deallocate(void* ptr, u64 size) {
    if(!ptr) {
        return;
    }

#ifndef NDEBUG
    constexpr u64 headerAlignment = alignof(Header);
    const u64 headerSize = align_up(sizeof(Header), headerAlignment);
    Header* header = reinterpret_cast<Header*>(static_cast<u8*>(ptr) - headerSize);
    header->state = HeaderState::Freed;

    m_usedSize -= header->size;
    m_freeSize += header->size;
    (void)size;
#else
    (void)size;
#endif

    auto* node = reinterpret_cast<FreeNode*>(ptr);
    node->next = m_freeList;
    m_freeList = node;
    m_freeCount++;
}

u64 PoolAllocator::aligned_chunk_size(u64 requested, u64 alignment) {
#ifndef NDEBUG
    constexpr u64 headerAlignment = alignof(Header);
    u64 headerSize = align_up(sizeof(Header), headerAlignment);
    return align_up(requested, alignment) + headerSize;
#else
    return align_up(requested, alignment);
#endif
}
