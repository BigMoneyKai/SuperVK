#pragma once

#include "core/define/types.h"
#include "stream.h"
#include <type_traits>

class OStream : public Stream {
public:
  virtual b32 write(const void *buf, u64 size) = 0;

  template <typename T>
    requires std::is_trivially_copyable_v<T>
  b32 write(const T &val) {
    return write(&val, sizeof(val));
  }
};
