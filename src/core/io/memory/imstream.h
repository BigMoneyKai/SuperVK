#pragma once

#include "defines.h"
#include "core/io/stream/istream.h"
#include "core/container/array/array.hpp"

class MemoryInputStream : public IStream {
public:
    MemoryInputStream(void* data, u64 size);
    u64 read(void* dst, u64 size) override;
    b32 is_open() const override;
    u64 size() const override;
    u64 position() const override;
    b32 seek(u64 offset, FileSeekOrigin origin) override;
    void close() override;

private:
    Array<u8> m_buffer;
    u64 m_size;
    u64 m_pos{0};
};
