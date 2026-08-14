#pragma once

#include "core/define/types.h"

enum class FileKind { file, directory };

struct FileState {
  u64 size;
  u64 modified;
  FileKind kind;
};
