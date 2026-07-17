#include "omstream.h"

void MemoryOutputStream::reserve(u64 size) {
    m_buffer.reserve(size);
}

b32 MemoryOutputStream::write(const void* rawData, u64 size) {
    m_buffer.append(static_cast<const u8*>(rawData), size);
    return SV_TRUE;
}

b32 MemoryOutputStream::is_open() const {
    return SV_TRUE;
}

u64 MemoryOutputStream::size() const {
    return m_buffer.size();
}

u64 MemoryOutputStream::position() const {
    return m_buffer.size();
}

b32 MemoryOutputStream::seek(u64 offset, FileSeekOrigin origin) {
    (void)offset;
    (void)origin;
    return SV_FALSE;
}

void MemoryOutputStream::close() {
    m_buffer.clear();
}

const u8* MemoryOutputStream::data() const {
    return m_buffer.data();
}

void MemoryOutputStream::clear() {
    m_buffer.clear();
}
