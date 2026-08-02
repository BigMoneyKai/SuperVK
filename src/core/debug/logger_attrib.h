#pragma once

#include "core/container/string/string.hpp"
#include "defines.h"

enum class LogLevel : u32 { Trace, Debug, Info, Warning, Error, Fatal };

enum class LogCatag : u32 {
  Container = 1 << 0,
  Memory = 1 << 1,
  Render = 1 << 2,
  IO = 1 << 2,
  Input = 1 << 3,
  Scene = 1 << 4,
  Vulkan = 1 << 5,
  Asset = 1 << 6,
  JobSystem = 1 << 7,
  FileSystem = 1 << 8,
  ECS = 1 << 9,
  Window = 1 << 10,
  Platform = 1 << 11,

  Unknown,

};

struct alignas(32) LogMsg {
  LogLevel level;
  LogCatag catag{LogCatag::Unknown};

  u64 id{UINT32_MAX};

  String msg;
  const char *func;
  const char *file;
  u32 line;

  u64 timestamp{0};
};

SV_STATIC_ASSERT(sizeof(LogMsg) == 96);
SV_STATIC_ASSERT(alignof(LogMsg) == 32);
