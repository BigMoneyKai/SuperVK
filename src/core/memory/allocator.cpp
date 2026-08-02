#include "allocator.h"
#include "alloc_counter.h"
#include "heap_allocator.h"
#include "utils/utils.h"

u64 AllocCounter::m_count = 0;

Allocator &default_allocator() { return HeapAllocator::instance(); }

#ifndef NDEBUG
u64 Allocator::totalSize() const { return m_totalSize; }

u64 Allocator::allocSize(void *ptr) const {
  constexpr u64 headerAlignment = alignof(Header);
  u64 headerSize = align_up(sizeof(Header), headerAlignment);
  u64 guardSize = sizeof(u32);
  u64 userOffset = headerSize + guardSize;
  Header *headerAddr =
      reinterpret_cast<Header *>(static_cast<u8 *>(ptr) - userOffset);
  return headerAddr->size;
}

u64 Allocator::usedSize() const { return m_usedSize; }

#else
u64 Allocator::totalSize() const { return 0; }

u64 Allocator::allocSize(void *ptr) const { return 0; }

u64 Allocator::usedSize() const { return 0; }
#endif
