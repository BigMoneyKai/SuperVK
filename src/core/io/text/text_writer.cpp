#include "text_writer.h"

TextWriter::TextWriter(BinaryWriter& writer)
    : m_writer(writer) {
}

void TextWriter::write(const char* str) {
    m_writer.write(str, std::char_traits<char>::length(str));
}

void TextWriter::write(const String& str) {
    m_writer.write(str.data(), str.size());
}

void TextWriter::write(char ch) {
    m_writer.write(&ch, 1);
}

void TextWriter::write_line(const char* str) {
    write(str);
    new_line();
}

void TextWriter::write_line(const String& str) {
    write(str);
    new_line();
}

void TextWriter::new_line() {
    char nl = '\n';
    m_writer.write(&nl, 1);
}

b32 TextWriter::is_valid() const {
    return m_writer.is_valid();
}
