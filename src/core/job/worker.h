#pragma once

#include "platform/thread.h"

class JobSystem;

struct Worker {
  void run();
  void steal();

  Thread thread;
  u32 id{0};
  JobSystem *system{nullptr};
};
