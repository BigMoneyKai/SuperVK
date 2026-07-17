#pragma once

#include "defines.h"
#include "platform/file.h"

class Stream {
public:
    virtual ~Stream() = default;
    virtual b32 is_open() const = 0;
    virtual u64 size() const = 0;
    virtual u64 position() const = 0;
    virtual b32 seek(u64 offset, FileSeekOrigin origin) = 0;
    virtual void close() = 0;
};
