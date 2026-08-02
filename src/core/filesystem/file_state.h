#pragma once

#include "defines.h"

enum class FileKind { File, Directory };

struct FileState {
  u64 size;
  u64 modified;
  FileKind kind;
};
