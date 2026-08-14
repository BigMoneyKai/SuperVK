#include "imstream.h"
#include <cstring>

MemoryInputStream::MemoryInputStream(void *data, u64 size)
    : m_buffer(data, size) {
  m_size = size;
}

u64 MemoryInputStream::read(void *dst, u64 size) {
  u64 bytes_to_read = size;
  if (m_pos + bytes_to_read > m_size)
    bytes_to_read = m_size - m_pos;
  if (bytes_to_read == 0)
    return 0;
  std::memcpy(dst, m_buffer.data() + m_pos, bytes_to_read);
  m_pos += bytes_to_read;
  return bytes_to_read;
}

b32 MemoryInputStream::is_open() const { return SV_TRUE; }

u64 MemoryInputStream::size() const { return m_size; }

u64 MemoryInputStream::position() const { return m_pos; }

b32 MemoryInputStream::seek(u64 offset, FileSeekOrigin origin) {
  switch (origin) {
  case FileSeekOrigin::begin:
    if (offset > m_size)
      return SV_FALSE;
    m_pos = offset;
    return SV_TRUE;
  case FileSeekOrigin::current: {
    i64 new_pos = static_cast<i64>(m_pos) + static_cast<i64>(offset);
    if (new_pos < 0 || static_cast<u64>(new_pos) > m_size)
      return SV_FALSE;
    m_pos = static_cast<u64>(new_pos);
    return SV_TRUE;
  }
  case FileSeekOrigin::end: {
    i64 new_pos = static_cast<i64>(m_size) + static_cast<i64>(offset);
    if (new_pos < 0 || static_cast<u64>(new_pos) > m_size)
      return SV_FALSE;
    m_pos = static_cast<u64>(new_pos);
    return SV_TRUE;
  }
  }
  return SV_FALSE;
}

void MemoryInputStream::close() { m_pos = 0; }
