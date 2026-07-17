#include "obstream.h"

BufferOutputStream::BufferOutputStream(OStream* output, u64 buffer_size)
    : m_output(output) {
    m_buffer.reserve(buffer_size > 0 ? buffer_size : 4096);
}

BufferOutputStream::~BufferOutputStream() {
    flush();
}

b32 BufferOutputStream::flush_buffer() {
    if (m_buffer.empty())
        return SV_TRUE;
    b32 ok = m_output->write(m_buffer.data(), m_buffer.size());
    m_buffer.clear();
    return ok;
}

b32 BufferOutputStream::write(const void* data, u64 size) {
    if (!m_output || !m_output->is_open())
        return SV_FALSE;

    m_buffer.append(static_cast<const u8*>(data), size);

    // Flush if buffer exceeds threshold
    if (m_buffer.size() >= m_buffer.capacity()) {
        return flush_buffer();
    }
    return SV_TRUE;
}

b32 BufferOutputStream::is_open() const {
    return m_output && m_output->is_open();
}

u64 BufferOutputStream::size() const {
    return m_output ? m_output->size() + m_buffer.size() : m_buffer.size();
}

u64 BufferOutputStream::position() const {
    return m_output ? m_output->position() + m_buffer.size() : m_buffer.size();
}

b32 BufferOutputStream::seek(u64 offset, FileSeekOrigin origin) {
    if (!m_output)
        return SV_FALSE;
    if (!flush_buffer())
        return SV_FALSE;
    return m_output->seek(offset, origin);
}

void BufferOutputStream::close() {
    flush();
    if (m_output) {
        m_output->close();
        m_output = nullptr;
    }
}

void BufferOutputStream::flush() {
    flush_buffer();
}
