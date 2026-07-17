#pragma once

#include "core/io/stream/ostream.h"
#include "core/container/array/array.hpp"

class BufferOutputStream : public OStream {
public:
    BufferOutputStream(OStream* output, u64 buffer_size = 4096);
    ~BufferOutputStream() override;

    b32 write(const void* data, u64 size) override;
    b32 is_open() const override;
    u64 size() const override;
    u64 position() const override;
    b32 seek(u64 offset, FileSeekOrigin origin) override;
    void close() override;
    void flush();

private:
    OStream* m_output;
    Array<u8> m_buffer;

    b32 flush_buffer();
};
