#include "text_reader.h"

TextReader::TextReader(BinaryReader& reader)
    : m_reader(reader) {
}

b32 TextReader::read_line(String& str) {
    str.clear();
    char ch;
    while (m_reader.read(&ch, 1) == 1) {
        if (ch == '\n') {
            m_prev_was_cr = SV_FALSE;
            return SV_TRUE;
        }
        if (ch == '\r') {
            m_prev_was_cr = SV_TRUE;
            continue;
        }
        str.push_back(ch);
    }
    return str.size() > 0 || m_reader.is_valid();
}

b32 TextReader::read_token(String& str) {
    str.clear();
    char ch;

    // skip leading whitespace
    while (m_reader.read(&ch, 1) == 1) {
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r')
            break;
    }

    if (!m_reader.is_valid() && str.empty() && ch == 0)
        return SV_FALSE;

    str.push_back(ch);

    // read until whitespace
    while (m_reader.read(&ch, 1) == 1) {
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
            break;
        str.push_back(ch);
    }

    return SV_TRUE;
}

b32 TextReader::is_valid() const {
    return m_reader.is_valid();
}

b32 TextReader::is_eof() const {
    return m_reader.is_eof();
}
