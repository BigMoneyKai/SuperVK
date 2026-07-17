#pragma once

#include "core/io/stream/istream.h"
#include "core/container/array/array.hpp"

class BufferInputStream : public IStream {
public:
    BufferInputStream(IStream* input, u64 buffer_size = 4096);
    ~BufferInputStream() override;

    u64 read(void* dst, u64 size) override;
    b32 is_open() const override;
    u64 size() const override;
    u64 position() const override;
    b32 seek(u64 offset, FileSeekOrigin origin) override;
    void close() override;
    void flush();

private:
    IStream* m_input;
    Array<u8> m_buffer;
    u64 m_buffer_size;
    u64 m_buf_pos{0};
    u64 m_buf_filled{0};

    u64 fill_buffer();
};
