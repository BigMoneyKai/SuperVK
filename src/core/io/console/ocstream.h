#pragma once

#include "defines.h"
#include "core/io/stream/ostream.h"
#include "core/io/serializer/serializer.h"
#include "platform/console.h"

class ConsoleOutputStream : public OStream {
public:
    b32 write(const void* buf, u64 size) override;
    b32 is_open() const override;
    u64 size() const override;
    u64 position() const override;
    b32 seek(u64 offset, FileSeekOrigin origin = FileSeekOrigin::FILE_SEEK_CURRENT) override;
    void close() override;

    template<typename T>
    ConsoleOutputStream& operator<<(const T& val) {
        serialize(*this, val);
        return *this;
    }
};
