#pragma once

#include "core/container/string/string.hpp"
#include "core/io/stream/istream.h"
#include <type_traits>

class BinaryReader {
public:
  explicit BinaryReader(IStream &stream);

  u64 read(void *dst, u64 size);
  b32 is_valid() const;
  b32 is_eof() const;

  template <typename T>
    requires std::is_trivially_copyable_v<T>
  BinaryReader &operator>>(T &val) {
    m_stream.read(val);
    return *this;
  }

  void read_string(String &str);

private:
  IStream &m_stream;
};
