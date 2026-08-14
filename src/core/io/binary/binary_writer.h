#pragma once

#include "core/container/string/string.hpp"
#include "core/io/stream/ostream.h"
#include <type_traits>

class BinaryWriter {
public:
  explicit BinaryWriter(OStream &stream);

  b32 write(const void *src, u64 size);
  b32 is_valid() const;

  template <typename T>
    requires std::is_trivially_copyable_v<T>
  BinaryWriter &operator<<(const T &val) {
    m_stream.write(val);
    return *this;
  }

  void write_string(const String &str);

private:
  OStream &m_stream;
};
