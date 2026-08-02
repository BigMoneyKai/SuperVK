#include "pool_allocator.h"
#include "alloc_counter.h"
#include "allocator_type.h"
#include "platform/memory.h"
#include "utils/utils.h"

PoolAllocator::PoolAllocator(u64 chunkSize, u64 chunkCount, u32 alignment,
                             Allocator *backing)
    : m_chunkSize(chunkSize), m_chunkCount(chunkCount), m_backing(backing) {
  m_chunkSize = align_up(chunkSize, alignment);

  u64 headerSize = align_up(sizeof(Header), alignof(Header));
  u64 guardSize = sizeof(u32);
  u64 userOffset = headerSize + guardSize;
  u64 chunkStride = userOffset + m_chunkSize + guardSize;

  u64 blockSize = chunkStride * m_chunkCount;

  if (m_backing) {
    m_start = static_cast<u8 *>(m_backing->allocate(blockSize, alignment));
  } else {
    m_start = static_cast<u8 *>(platform_aligned_alloc(blockSize, alignment));
  }

  SV_ASSERT(m_start, "Allocator buffer not initialized");

  m_totalSize = blockSize;
  m_usedSize = 0;
  m_freeCount = m_chunkCount;

  for (u64 i = 0; i < m_chunkCount; i++) {
    u8 *chunk = static_cast<u8 *>(m_start) + i * chunkStride;
    u8 *userData = chunk + userOffset;

    auto *node = reinterpret_cast<FreeNode *>(userData);
    node->next = m_freeList;
    m_freeList = node;
  }
}

PoolAllocator::~PoolAllocator() {
  if (!m_start) {
    return;
  }
  m_freeList = nullptr;

  if (m_backing) {
    m_backing->deallocate(m_start);
  } else {
    platform_aligned_free(m_start);
  }
}

void *PoolAllocator::allocate(u64 size, u64 alignment) {
  if (size > m_chunkSize || alignment > m_chunkSize) {
    return nullptr;
  }
  if (!m_freeList) {
    return nullptr;
  }

  FreeNode *node = m_freeList;
  m_freeList = node->next;
  m_freeCount--;

  u64 headerSize = align_up(sizeof(Header), alignof(Header));
  u64 guardSize = sizeof(u32);
  u64 userOffset = headerSize + guardSize;

  void *userPtr = node;
  u8 *headerPtr = static_cast<u8 *>(userPtr) - userOffset;

  Header *header = reinterpret_cast<Header *>(headerPtr);
  header->size = size;
  header->alignment = alignment;
  header->allocationId = AllocCounter::add();
  header->allocatorId = AllocatorType::Pool;
  header->state = HeaderState::Allocated;
  header->magic = HeaderState::Magic;

  HeaderState *frontGuard =
      reinterpret_cast<HeaderState *>(headerPtr + headerSize);
  *frontGuard = HeaderState::FrontGuard;
  HeaderState *backGuard =
      reinterpret_cast<HeaderState *>(static_cast<u8 *>(userPtr) + size);
  *backGuard = HeaderState::BackGuard;

  m_usedSize += size;
  return userPtr;
}

void PoolAllocator::deallocate(void *ptr) {
  if (!ptr) {
    return;
  }

  u64 headerSize = align_up(sizeof(Header), alignof(Header));
  u64 guardSize = sizeof(u32);
  u64 userOffset = headerSize + guardSize;

  u8 *userPtr = static_cast<u8 *>(ptr);
  u8 *headerPtr = userPtr - userOffset;
  Header *header = reinterpret_cast<Header *>(headerPtr);

  SV_ASSERT(header->magic == HeaderState::Magic, "Invalid allocation");

  HeaderState *frontGuard =
      reinterpret_cast<HeaderState *>(headerPtr + headerSize);
  SV_ASSERT(static_cast<u32>(*frontGuard) == FRONT_GUARD,
            "Front guard corrupted");
  HeaderState *backGuard =
      reinterpret_cast<HeaderState *>(userPtr + header->size);
  SV_ASSERT(static_cast<u32>(*backGuard) == BACK_GUARD,
            "Back guard corrupted");

  SV_ASSERT(header->state == HeaderState::Allocated, "Double free detected");
  header->state = HeaderState::Freed;

  m_usedSize -= header->size;

  auto *node = reinterpret_cast<FreeNode *>(ptr);
  node->next = m_freeList;
  m_freeList = node;
  m_freeCount++;
}

u64 PoolAllocator::aligned_chunk_size(u64 requested, u64 alignment) {
  u64 headerSize = align_up(sizeof(Header), alignof(Header));
  u64 guardSize = sizeof(u32);
  u64 userOffset = headerSize + guardSize;
  return align_up(requested, alignment) + userOffset + guardSize;
}
