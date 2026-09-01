#pragma once

#include "core/define/types.h"

namespace Platform {
u64 console_read(void *buffer, u64 size);
u64 console_write(const void *buffer, u64 size);
} // namespace Platform

using Platform::console_read;
using Platform::console_write;
