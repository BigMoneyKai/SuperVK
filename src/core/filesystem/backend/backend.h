#pragma once

#include "core/filesystem/file_path.h"
#include "core/filesystem/file_state.h"
#include "core/io/buffer/obstream.h"
#include "defines.h"
#include "platform/file.h"
class FsBackend {
public:
  virtual ~FsBackend() = default;

  virtual b32 exists(const FilePath &path) = 0;
  virtual b32 open(const FilePath &path, FileState *outState) = 0;
  virtual FileHandle *open(const FilePath &path, FileMode mode) = 0;

  virtual b32 readFile(const FilePath &path, Allocator *a,
                       BufferOutputStream *out) = 0;
  virtual b32 writeFile(const FilePath &path, const void *data, u64 size) = 0;
  virtual b32 deleteFile(const FilePath &path) = 0;
};
