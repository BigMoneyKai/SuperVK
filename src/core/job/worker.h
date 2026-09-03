#pragma once

#include "core/define/arch.h"
#include "platform/thread.h"

class JobSystem;

struct alignas(SV_CACHE_LINE_SIZE) Worker {
  void run();
  void steal();

  Thread thread;
  u32 id{0};
  JobSystem *system{nullptr};
};
