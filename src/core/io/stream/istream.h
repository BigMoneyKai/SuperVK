#pragma once

#include "defines.h"
#include "stream.h"
#include <type_traits>

class IStream : public Stream {
public:
    virtual u64 read(void* dst, u64 size) = 0;

    template<typename T>
    requires std::is_trivially_copyable_v<T>
    u64 read(T& val) {
        return read(&val, sizeof(val));
    }

};