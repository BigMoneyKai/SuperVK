#pragma once

#include "core/filesystem/backend/backend.h"

class NativeFsBackend : public FsBackend {
public:
  ~NativeFsBackend() override = default;

  b32 exists(const FilePath &path) override;
  b32 open(const FilePath &path, FileState *outState) override;
  FileHandle *open(const FilePath &path, FileMode mode) override;
  b32 readFile(const FilePath &path, Allocator *a,
               BufferOutputStream *out) override;
  b32 writeFile(const FilePath &path, const void *data, u64 size) override;
  b32 deleteFile(const FilePath &path) override;
};

NativeFsBackend &default_fsbackend();
