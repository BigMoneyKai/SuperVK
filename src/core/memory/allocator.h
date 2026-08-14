#pragma once

#include "allocator_type.h"
#include "core/define/types.h"
#include "core/memory/allocation_info.h"
#include <cstddef>
#include <unordered_map>

class Allocator {
public:
  virtual ~Allocator() = default;

  virtual void *allocate(u64 size, u64 alignment = DEFAULT_ALIGNMENT) = 0;
  virtual void deallocate(void *ptr) {};

  u64 totalSize() const;
  u64 allocSize(void *) const;
  u64 usedSize() const;

  void validateAllocations();

  static constexpr u64 DEFAULT_ALIGNMENT = 16;
  static constexpr u32 HEADER_MAGIC = 0x20040827;

protected:
  enum class HeaderState : u32 {
    allocated = 0xACCE55ED,
    freed = 0xF7EEF7EE,
    frontGuard = 0xDEADC0DE,
    backGuard = 0xBAADC0DE,

    // :33333 oo
    magic = 0xB19B00B5,
  };
  SV_STATIC_ASSERT(sizeof(HeaderState) == sizeof(u32));
  SV_STATIC_ASSERT(static_cast<u32>(HeaderState::magic) == 0xB19B00B5, ":(");

  struct alignas(32) Header {
    u64 size;
    u64 allocationId;
    HeaderState magic;
    HeaderState state;
    AllocatorType allocatorId;
    u32 alignment;
  };
  SV_STATIC_ASSERT(alignof(Header) == 32);

protected:
  u64 m_totalSize{0};
  u64 m_usedSize{0};

#ifndef NDEBUG
  std::unordered_map<void *, AllocationInfo> m_allocInfoRegistry;
#endif

public:
  static constexpr u32 FRONT_GUARD = static_cast<u32>(HeaderState::frontGuard);
  static constexpr u32 BACK_GUARD = static_cast<u32>(HeaderState::backGuard);
};

// Default global allocator
Allocator &default_allocator();
