#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"
#include "core/io/stream/istream.h"
#include "core/io/stream/ostream.h"
#include "serializer.h"

#include <type_traits>

template <typename T>
  requires std::is_trivially_copyable_v<T>
void serialize(OStream &os, const T &val) {
  os.write(val);
}

SV_FORCE_INLINE void serialize(OStream &os, const String &str) {
  u64 size = str.size();
  os.write(size);
  if (size > 0)
    os.write(str.data(), size);
}

SV_FORCE_INLINE void serialize(OStream &os, const char *str) {
  u64 size = strlen(str);
  os.write(size);
  if (size > 0)
    os.write(str, size);
}

template <typename T>
  requires std::is_trivially_copyable_v<T>
void deserialize(IStream &is, T &val) {
  is.read(val);
}

SV_FORCE_INLINE void deserialize(IStream &is, String &str) {
  is.read(str.data(), str.size());
}
SV_FORCE_INLINE void deserialize(IStream &is, char *str) {
  is.read(str, strlen(str));
}
