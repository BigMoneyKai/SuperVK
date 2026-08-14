#include "ifstream.h"
#include "platform/file.h"

FileHandle *FileInputStream::open(const char *path, FileMode mode) {
  return file_open(path, mode);
}

u64 FileInputStream::read(void *buf, u64 size) {
  return file_read(m_file, buf, size);
}

b32 FileInputStream::is_open() const { return file_is_open(m_file); }

u64 FileInputStream::size() const { return file_size(m_file); }

u64 FileInputStream::position() const { return file_tell(m_file); }

b32 FileInputStream::seek(u64 offset, FileSeekOrigin origin) {
  return file_seek(m_file, static_cast<i64>(offset), origin) == 0 ? SV_TRUE
                                                                  : SV_FALSE;
}

void FileInputStream::close() { file_close(m_file); }
