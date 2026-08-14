#pragma once

#include "allocator.h"

class PoolAllocator : public Allocator {
public:
  PoolAllocator(u64 chunkSize, u64 chunkCount, u32 alignment,
                Allocator *backing = &default_allocator());
  ~PoolAllocator() override;

  PoolAllocator(const PoolAllocator &) = delete;
  PoolAllocator &operator=(const PoolAllocator &) = delete;

  void *allocate(u64 size, u64 alignment = DEFAULT_ALIGNMENT) override;
  void deallocate(void *ptr) override;

private:
  struct FreeNode {
    FreeNode *next;
  };
  FreeNode *m_freeList{nullptr};
  u64 m_freeCount;

  void *m_start{nullptr};
  u64 m_chunkSize;
  u64 m_chunkCount;

  Allocator *m_backing{nullptr};

  static u64 aligned_chunk_size(u64 requested, u64 alignment);
};
