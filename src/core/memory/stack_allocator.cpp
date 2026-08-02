#include "stack_allocator.h"
#include "alloc_counter.h"
#include "allocator_type.h"
#include "platform/memory.h"
#include "utils/utils.h"

StackAllocator::StackAllocator(u64 size, Allocator *backing) {
  m_size = size;
  m_cursor = 0;
  m_backing = backing;
  m_totalSize = size;

  if (backing) {
    m_buffer =
        static_cast<u8 *>(backing->allocate(size, alignof(std::max_align_t)));
  } else {
    m_buffer = static_cast<u8 *>(
        platform_aligned_alloc(size, alignof(std::max_align_t)));
  }
}

StackAllocator::~StackAllocator() {
  if (!m_buffer) {
    return;
  }

  if (m_backing) {
    m_backing->deallocate(m_buffer);
  } else {
    platform_aligned_free(m_buffer);
  }
}

void *StackAllocator::allocate(u64 size, u64 alignment) {
  u64 headerSize = align_up(sizeof(Header), alignof(Header));
  u64 guardSize = sizeof(u32);
  u64 userOffset = headerSize + guardSize;

  u64 headerOffset = align_up(m_cursor, alignof(Header));
  u64 userDataEnd = headerOffset + userOffset + size + guardSize;

  if (userDataEnd > m_size) {
    return nullptr;
  }

  u8 *base = static_cast<u8 *>(m_buffer);
  u8 *headerPtr = base + headerOffset;
  u8 *userPtr = headerPtr + userOffset;

  Header *header = reinterpret_cast<Header *>(headerPtr);
  header->size = size;
  header->alignment = alignment;
  header->allocationId = AllocCounter::add();
  header->allocatorId = AllocatorType::Stack;
  header->state = HeaderState::Allocated;
  header->magic = HeaderState::Magic;

  HeaderState *frontGuard =
      reinterpret_cast<HeaderState *>(headerPtr + headerSize);
  *frontGuard = HeaderState::FrontGuard;
  HeaderState *backGuard =
      reinterpret_cast<HeaderState *>(headerPtr + userOffset + size);
  *backGuard = HeaderState::BackGuard;

  m_usedSize += size;
  m_cursor = userDataEnd;

  return userPtr;
}

void StackAllocator::deallocate(void *ptr) {
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
      reinterpret_cast<HeaderState *>(headerPtr + userOffset + header->size);
  SV_ASSERT(static_cast<u32>(*backGuard) == BACK_GUARD,
            "Back guard corrupted");

  SV_ASSERT(header->state == HeaderState::Allocated, "Double free detected");
  header->state = HeaderState::Freed;

  u64 headerOffset =
      static_cast<u64>(headerPtr - static_cast<u8 *>(m_buffer));
  m_cursor = headerOffset;
  m_usedSize -= header->size;
}

void StackAllocator::reset_to(Marker marker) { m_cursor = marker.cursor; }

void StackAllocator::reset() {
  m_cursor = 0;
  m_usedSize = 0;
}
