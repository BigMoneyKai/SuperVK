#pragma once

#include "defines.h"
#include "core/container/string/string.hpp"
#include "core/container/array/array.hpp"
#include <filesystem>
#include <fstream>
#include <span>

class CacheMan {
public:
    explicit CacheMan(const String& dir);

    b32 store(const String& key, std::span<const u8> data);
    b32 load(const String& key, Array<u8>& out);
    b32 exists(const String& key);

private:
    static String sanitize(const String& key) {
        String result;
        for (char c : key) {
            if (std::isalnum(static_cast<u8>(c)) || c == '.' || c == '_')
                result += c;
            else
                result += '_';
        }
        return result;
    }

    String m_cacheDir;
};
