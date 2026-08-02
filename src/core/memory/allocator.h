#pragma once

#include "allocator_type.h"
#include "defines.h"
#include <cstddef>

class Allocator {
public:
  virtual ~Allocator() = default;

  virtual void *allocate(u64 size, u64 alignment = DEFAULT_ALIGNMENT) = 0;
  virtual void deallocate(void *ptr) = 0;

  u64 totalSize() const;
  u64 allocSize(void *) const;
  u64 usedSize() const;

  static constexpr u64 DEFAULT_ALIGNMENT = 16;
  static constexpr u32 HEADER_MAGIC = 0x20040827;

protected:
  enum class HeaderState : u32 {
    Allocated = 0xACCE55ED,
    Freed = 0xF7EEF7EE,
    FrontGuard = 0xDEADC0DE,
    BackGuard = 0xBAADC0DE,

    // :33333 oo
    Magic = 0xB19B00B5,
  };
  SV_STATIC_ASSERT(sizeof(HeaderState) == sizeof(u32));
  SV_STATIC_ASSERT(static_cast<u32>(HeaderState::Magic) == 0xB19B00B5, ":(");

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

public:
  static constexpr u32 FRONT_GUARD = static_cast<u32>(HeaderState::FrontGuard);
  static constexpr u32 BACK_GUARD = static_cast<u32>(HeaderState::BackGuard);
};

// Default global allocator
Allocator &default_allocator();
