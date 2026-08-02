#pragma once

#include "core/container/hashmap/hashmap.hpp"
#include "core/filesystem/backend/backend.h"
#include "core/filesystem/file_path.h"
#include "defines.h"

struct PakEntry {
  u64 offset;
  u64 size;
};

class MemFsBackend : public FsBackend {
public:
  ~MemFsBackend() = default;

private:
  HashMap<FilePath, Buffer> m_files;
};
