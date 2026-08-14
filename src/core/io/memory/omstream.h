#pragma once

#include "core/container/array/array.hpp"
#include "core/io/stream/ostream.h"

class MemoryOutputStream : public OStream {
public:
  void reserve(u64 size);
  b32 write(const void *rawData, u64 size) override;
  b32 is_open() const override;
  u64 size() const override;
  u64 position() const override;
  b32 seek(u64 offset, FileSeekOrigin origin) override;
  void close() override;

  const u8 *data() const;
  void clear();

private:
  Array<u8> m_buffer;
};
