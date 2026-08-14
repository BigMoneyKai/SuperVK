#pragma once

#include "core/container/array/array.hpp"
#include <cstring>

struct AssetSerializer {
  template <typename T> static b32 write(const T &val, Array<u8> &out) {
    auto *ptr = reinterpret_cast<const u8 *>(&val);
    for (u64 i = 0; i < sizeof(T); ++i)
      out.push_back(ptr[i]);
    return true;
  }

  template <typename T>
  static b32 read(const Array<u8> &data, u64 &offset, T &val) {
    if (offset + sizeof(T) > data.size())
      return false;
    std::memcpy(&val, data.data() + offset, sizeof(T));
    offset += sizeof(T);
    return true;
  }

  template <typename T>
  static b32 writeArray(const Array<T> &arr, Array<u8> &out) {
    if (!write(arr.size(), out))
      return false;
    for (u64 i = 0; i < arr.size(); ++i)
      if (!write(arr[i], out))
        return false;
    return true;
  }

  template <typename T>
  static b32 readArray(const Array<u8> &data, u64 &offset, Array<T> &arr) {
    u64 size = 0;
    if (!read(data, offset, size))
      return false;
    arr.resize(size);
    for (u64 i = 0; i < size; ++i)
      if (!read(data, offset, arr[i]))
        return false;
    return true;
  }
};
