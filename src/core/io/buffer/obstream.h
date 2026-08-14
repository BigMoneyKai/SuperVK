#pragma once

#include "core/container/array/array.hpp"
#include "core/io/stream/ostream.h"

class BufferOutputStream : public OStream {
public:
  BufferOutputStream(OStream *output, u64 buffer_size = DEFAULT_SIZE);
  ~BufferOutputStream() override;

  b32 write(const void *data, u64 size) override;
  b32 is_open() const override;
  u64 size() const override;
  u64 position() const override;
  b32 seek(u64 offset, FileSeekOrigin origin) override;
  void close() override;
  void flush();

  static constexpr u64 DEFAULT_SIZE = 4096;

private:
  OStream *m_output;
  Array<u8> m_buffer;

  b32 flush_buffer();
};
