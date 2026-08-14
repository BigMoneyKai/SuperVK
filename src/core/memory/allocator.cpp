#include "allocator.h"
#include "alloc_counter.h"
#include "allocation_info.h"
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

void Allocator::validateAllocations() {
  u64 headerSize = align_up(sizeof(Header), alignof(Header));
  u64 guardSize = sizeof(u32);

  for (const auto &it : m_allocInfoRegistry) {
    const u8 *ptr = static_cast<u8 *>(it.first);
    const auto &info = it.second;

    const Header *header =
        reinterpret_cast<const Header *>(ptr - guardSize - headerSize);
    const HeaderState *frontGuard =
        reinterpret_cast<const HeaderState *>(ptr - guardSize);
    const HeaderState *backGuard =
        reinterpret_cast<const HeaderState *>(ptr + info.size);

    SV_ASSERT(info.alignment == header->alignment, "Wrong alignment");
    SV_ASSERT(info.size == header->size, "Wrong size");
    SV_ASSERT(header->magic == HeaderState::magic,
              "Allocated without allocator");
    SV_ASSERT(static_cast<u32>(*frontGuard) == FRONT_GUARD,
              "Memory write underflow");
    SV_ASSERT(static_cast<u32>(*backGuard) == BACK_GUARD,
              "Memory write overflow");
  }
}
#else
u64 Allocator::totalSize() const { return 0; }

u64 Allocator::allocSize(void *ptr) const { return 0; }

u64 Allocator::usedSize() const { return 0; }
void Allocator::validateAllocations() { return; }
#endif
