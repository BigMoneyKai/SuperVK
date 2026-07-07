#pragma once

#include "defines.h"
#include <cstdlib>

// aligned allocation and freeing
void* platform_aligned_alloc(u64 size, u64 alignment);
void  platform_aligned_free(void* ptr);

// operate big chunk of virtual memory
void* virtual_reserve(u64 size);
void  virtual_release(void* ptr, u64 size);
void  virtual_commit(void* ptr, u64 size);
void  virtual_decommit(void* ptr, u64 size);

// memory page size and granularity
u64 page_size();
u64 allocation_granularity();

