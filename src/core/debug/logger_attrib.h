#pragma once

#include "core/container/string/string.hpp"
#include "core/define/assert.h"
#include "core/define/types.h"
#include <new>

enum class LogLevel : u32 { trace, debug, info, warning, error, fatal };

enum class LogCatag : u32 {
  container = 0,
  memory,
  render,
  inputOutput,
  input,
  scene,
  vulkan,
  asset,
  jobSystem,
  fileSystem,
  entityComponentSystem,
  window,
  platform,
  ui,

  unknown,

};

struct alignas(std::hardware_destructive_interference_size) LogMsg {
  LogLevel level;
  LogCatag catag{LogCatag::unknown};

  u64 id{UINT32_MAX};

  String msg;
  const char *func;
  const char *file;
  u32 line;

  u64 timestamp{0};
};
