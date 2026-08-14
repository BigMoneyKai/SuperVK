#pragma once

#include "core/container/array/array.hpp"
#include "core/container/string/string_view.hpp"
#include "mount/file_mount.h"
#include "platform/file.h"

class FileSystem {
public:
  virtual FileHandle open(StringView path, FileMode mode) = 0;
  virtual void close(StringView path) = 0;

private:
  Array<FileMount *> m_mounts;
  String m_root;
};
