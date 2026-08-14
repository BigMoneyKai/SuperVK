#pragma once

#include "core/define/types.h"

struct AllocationInfo {
  u64 size;
  u64 alignment;

  const char *file;
  int line;
};
