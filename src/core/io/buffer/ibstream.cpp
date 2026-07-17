#include "ibstream.h"
#include <cstring>

BufferInputStream::BufferInputStream(IStream* input, u64 buffer_size)
    : m_input(input)
    , m_buffer(buffer_size > 0 ? buffer_size : 4096)
    , m_buffer_size(buffer_size > 0 ? buffer_size : 4096) {
}

BufferInputStream::~BufferInputStream() {
    close();
}

u64 BufferInputStream::fill_buffer() {
    m_buf_pos = 0;
    m_buf_filled = m_input->read(m_buffer.data(), m_buffer_size);
    return m_buf_filled;
}

u64 BufferInputStream::read(void* dst, u64 size) {
    if (!m_input || !m_input->is_open())
        return 0;

    u64 total_read = 0;
    auto* bytes = static_cast<u8*>(dst);

    while (size > 0) {
        if (m_buf_pos >= m_buf_filled) {
            if (fill_buffer() == 0)
                break;
        }

        u64 available = m_buf_filled - m_buf_pos;
        u64 to_copy = size < available ? size : available;
        std::memcpy(bytes + total_read, m_buffer.data() + m_buf_pos, to_copy);
        m_buf_pos += to_copy;
        total_read += to_copy;
        size -= to_copy;
    }

    return total_read;
}

b32 BufferInputStream::is_open() const {
    return m_input && m_input->is_open();
}

u64 BufferInputStream::size() const {
    return m_input ? m_input->size() : 0;
}

u64 BufferInputStream::position() const {
    // Can't accurately track without seeking
    return m_input ? m_input->position() : 0;
}

b32 BufferInputStream::seek(u64 offset, FileSeekOrigin origin) {
    if (!m_input)
        return SV_FALSE;
    m_buf_pos = 0;
    m_buf_filled = 0;
    return m_input->seek(offset, origin);
}

void BufferInputStream::close() {
    if (m_input) {
        m_input->close();
        m_input = nullptr;
    }
    m_buf_pos = 0;
    m_buf_filled = 0;
    m_buffer.clear();
}

void BufferInputStream::flush() {
    m_buf_pos = 0;
    m_buf_filled = 0;
}
