#pragma once

#include "platform/file.h"

class File {
public:
private:
  FileHandle *m_file;
  FileMode m_mode;
};
