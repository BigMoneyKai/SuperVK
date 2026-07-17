#pragma once

#include "defines.h"
#include "core/io/binary/binary_reader.h"
#include "core/container/string/string.hpp"
#include <cstdlib>
#include <type_traits>

class TextReader {
public:
    explicit TextReader(BinaryReader& reader);

    b32 read_line(String& str);
    b32 read_token(String& str);
    b32 is_valid() const;
    b32 is_eof() const;

    template<typename T>
    TextReader& operator>>(T& val) {
        String token;
        read_token(token);
        if constexpr (std::is_floating_point_v<T>)
            val = static_cast<T>(std::strtod(token.c_str(), nullptr));
        else if constexpr (std::is_signed_v<T>)
            val = static_cast<T>(std::strtoll(token.c_str(), nullptr, 10));
        else
            val = static_cast<T>(std::strtoull(token.c_str(), nullptr, 10));
        return *this;
    }

private:
    BinaryReader& m_reader;
    b32 m_prev_was_cr{SV_FALSE};
};
