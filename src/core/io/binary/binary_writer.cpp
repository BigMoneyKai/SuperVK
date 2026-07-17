#include "binary_writer.h"

BinaryWriter::BinaryWriter(OStream& stream)
    : m_stream(stream) {
}

b32 BinaryWriter::write(const void* src, u64 size) {
    return m_stream.write(src, size);
}

b32 BinaryWriter::is_valid() const {
    return m_stream.is_open();
}

void BinaryWriter::write_string(const String& str) {
    u64 len = str.size();
    m_stream.write(len);
    if (len > 0)
        m_stream.write(str.data(), len);
}
