#pragma once

#include "core/define/types.h"
#include <cstdlib>

namespace Platform {
// aligned allocation and freeing
void *platform_aligned_alloc(u64 size, u64 alignment);
void platform_aligned_free(void *ptr);

// operate big chunk of virtual memory
void *virtual_reserve(u64 size);
void virtual_release(void *ptr, u64 size);
void virtual_commit(void *ptr, u64 size);
void virtual_decommit(void *ptr, u64 size);

// memory page size and granularity
u64 page_size();
u64 allocation_granularity();
} // namespace Platform

using Platform::platform_aligned_alloc;
using Platform::platform_aligned_free;
using Platform::virtual_reserve;
using Platform::virtual_release;
using Platform::virtual_commit;
using Platform::virtual_decommit;
using Platform::page_size;
using Platform::allocation_granularity;
