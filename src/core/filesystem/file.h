#pragma once

#include "file_info.h"
#include "platform/file.h"

class File {
public:
private:
  FileHandle *m_file;
  FileMode m_mode;
};
