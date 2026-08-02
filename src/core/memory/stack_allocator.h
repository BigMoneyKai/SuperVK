#pragma once

#include "allocator.h"

struct Marker {
  u64 cursor;
};

class StackAllocator : public Allocator {
public:
  StackAllocator(u64 size, Allocator *backing = &default_allocator());
  ~StackAllocator();

  void *allocate(u64 size, u64 alignment = DEFAULT_ALIGNMENT) override;
  void deallocate(void *ptr) override;

  void reset_to(Marker marker);
  void reset();

  SV_FORCE_INLINE u64 used() const { return m_cursor; }

  SV_FORCE_INLINE u64 capacity() const { return m_size; }
  SV_FORCE_INLINE u64 remaining() const { return m_size - m_cursor; }

private:
  void *m_buffer{nullptr};
  u64 m_size;
  u64 m_cursor;
  Allocator *m_backing{nullptr};
};
