#pragma once

#include "defines.h"
#include "serializer.h"
#include "core/io/stream/istream.h"
#include "core/io/stream/ostream.h"
#include <type_traits>

template<typename T>
requires std::is_trivially_copyable_v<T>
void serialize(OStream& os, const T& val) {
    os.write(val);
}

void serialize(OStream& os, const String& val) {
    u64 size = val.size();
    os.write(size);
    if (size > 0)
        os.write(val.data(), size);
}

template<typename T>
requires std::is_trivially_copyable_v<T>
void deserialize(IStream& is, T& val) {
    is.read(val);
}

void deserialize(IStream& is, String& val) {
    u64 size = 0;
    is.read(size);
    if (size > 0) {
        val.resize(size);
        is.read(val.data(), size);
    }
}
