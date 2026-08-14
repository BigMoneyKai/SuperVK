#include "linear_allocator.h"
#include "alloc_counter.h"
#include "allocator_type.h"
#include "platform/memory.h"
#include "utils/utils.h"

LinearAllocator::LinearAllocator(u64 size, Allocator *backing) {
  m_size = size;
  m_totalSize = size;
  m_cursor = 0;
  m_backing = backing;

  if (backing) {
    m_buffer =
        static_cast<u8 *>(backing->allocate(size, alignof(std::max_align_t)));
  } else {
    m_buffer = static_cast<u8 *>(
        platform_aligned_alloc(size, alignof(std::max_align_t)));
  }

  SV_ASSERT(m_buffer, "Allocator buffer not initialized");
}

LinearAllocator::~LinearAllocator() {
  if (!m_buffer) {
    return;
  }

  if (m_backing) {
    m_backing->deallocate(m_buffer);
    m_totalSize = 0;
    m_usedSize = 0;
  } else {
    platform_aligned_free(m_buffer);
  }
}

void *LinearAllocator::allocate(u64 size, u64 alignment) {
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
  header->allocatorId = AllocatorType::linear;
  header->state = HeaderState::allocated;
  header->magic = HeaderState::magic;

  HeaderState *frontGuard =
      reinterpret_cast<HeaderState *>(headerPtr + headerSize);
  *frontGuard = HeaderState::frontGuard;
  HeaderState *backGuard =
      reinterpret_cast<HeaderState *>(headerPtr + userOffset + size);
  *backGuard = HeaderState::backGuard;

  m_usedSize += size;
  m_cursor = userDataEnd;

  return userPtr;
}

void LinearAllocator::reset() {
  m_cursor = 0;

  m_usedSize = 0;
}
