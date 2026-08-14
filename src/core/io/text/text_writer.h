#pragma once

#include "core/container/string/string.hpp"
#include "core/io/binary/binary_writer.h"
#include <cstdio>
#include <type_traits>

class TextWriter {
public:
  explicit TextWriter(BinaryWriter &writer);

  void write(const char *str);
  void write(const String &str);
  void write(char ch);
  void write_line(const char *str);
  void write_line(const String &str);
  void new_line();

  b32 is_valid() const;

  template <typename T> TextWriter &operator<<(const T &val) {
    String s = format_value(val);
    m_writer.write(s.data(), s.size());
    return *this;
  }

private:
  BinaryWriter &m_writer;

  template <typename T> static String format_value(T val) {
    if constexpr (std::is_floating_point_v<T>) {
      char buf[64];
      i32 n = std::snprintf(buf, sizeof(buf), "%g", static_cast<f64>(val));
      return String(buf, static_cast<u64>(n));
    } else if constexpr (std::is_signed_v<T>) {
      if (val == 0)
        return String("0");
      u64 abs_val = val < 0 ? -static_cast<u64>(val) : static_cast<u64>(val);
      char buf[32];
      char *p = buf + 31;
      *p = '\0';
      u64 u = abs_val;
      while (u > 0) {
        *--p = '0' + (u % 10);
        u /= 10;
      }
      if (val < 0)
        *--p = '-';
      return String(p);
    } else {
      if (val == 0)
        return String("0");
      char buf[32];
      char *p = buf + 31;
      *p = '\0';
      u64 u = static_cast<u64>(val);
      while (u > 0) {
        *--p = '0' + (u % 10);
        u /= 10;
      }
      return String(p);
    }
  }
};

template <> inline TextWriter &TextWriter::operator<<(const char *const &val) {
  m_writer.write(val, std::char_traits<char>::length(val));
  return *this;
}

template <> inline TextWriter &TextWriter::operator<<(const String &val) {
  m_writer.write(val.data(), val.size());
  return *this;
}

template <> inline TextWriter &TextWriter::operator<<(const char &val) {
  m_writer.write(&val, 1);
  return *this;
}
