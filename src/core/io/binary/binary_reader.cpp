#include "binary_reader.h"

BinaryReader::BinaryReader(IStream& stream)
    : m_stream(stream) {
}

u64 BinaryReader::read(void* dst, u64 size) {
    return m_stream.read(dst, size);
}

b32 BinaryReader::is_valid() const {
    return m_stream.is_open();
}

b32 BinaryReader::is_eof() const {
    return m_stream.position() >= m_stream.size();
}

void BinaryReader::read_string(String& str) {
    u64 len = 0;
    m_stream.read(len);
    if (len > 0) {
        str.resize(len);
        m_stream.read(str.data(), len);
    }
}
