#pragma once

#include "core/define/types.h"

u64 console_read(void *buffer, u64 size);
u64 console_write(const void *buffer, u64 size);
