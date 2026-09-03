#include "heap_allocator.h"
#include "alloc_counter.h"
#include "allocator_type.h"
#include "platform/memory.h"
#include "utils/utils.h"

HeapAllocator &HeapAllocator::instance() {
  static HeapAllocator s_instance;
  return s_instance;
}

void *HeapAllocator::allocate(u64 size, u64 alignment) {
  u64 allocAlignment =
      alignment > alignof(Header) ? alignment : alignof(Header);
  u64 headerSize = align_up(sizeof(Header), alignof(Header));
  u64 guardSize = sizeof(u32);
  u64 userOffset = headerSize + guardSize;

  u8 *realPtr = static_cast<u8 *>(platform_aligned_alloc(
      userOffset + guardSize + size + guardSize, allocAlignment));

  u8 *userPtr = realPtr + userOffset;

  Header *header = reinterpret_cast<Header *>(realPtr);
  header->size = size;
  header->alignment = static_cast<u32>(allocAlignment);
  header->allocationId = AllocCounter::add();
  header->allocatorId = AllocatorType::heap;
  header->state = HeaderState::allocated;
  header->magic = HeaderState::magic;

  HeaderState *frontGuard =
      reinterpret_cast<HeaderState *>(realPtr + headerSize);
  *frontGuard = HeaderState::frontGuard;
  HeaderState *backGuard =
      reinterpret_cast<HeaderState *>(realPtr + headerSize + guardSize + size);
  *backGuard = HeaderState::backGuard;

  m_usedSize += size;

  return userPtr;
}

void HeapAllocator::deallocate(void *ptr) {
  if (!ptr) {
    return;
  }
  u64 headerSize = align_up(sizeof(Header), alignof(Header));
  u64 guardSize = sizeof(u32);
  u64 userOffset = headerSize + guardSize;

  u8 *realPtr = reinterpret_cast<u8 *>(static_cast<u8 *>(ptr) - userOffset);
  Header *header = reinterpret_cast<Header *>(realPtr);
  SV_ASSERT(header->magic == HeaderState::magic, "Invalid allocation");

  HeaderState *frontGuard =
      reinterpret_cast<HeaderState *>(realPtr + headerSize);
  SV_ASSERT(static_cast<u32>(*frontGuard) == FRONT_GUARD,
            "Front guard corrupted");
  HeaderState *backGuard = reinterpret_cast<HeaderState *>(
      realPtr + headerSize + guardSize + header->size);
  SV_ASSERT(static_cast<u32>(*backGuard) == BACK_GUARD, "Back guard corrupted");

  SV_ASSERT(header->state == HeaderState::allocated, "Double free detected");
  header->state = HeaderState::freed;

  m_usedSize -= header->size;
  platform_aligned_free(realPtr);
}
