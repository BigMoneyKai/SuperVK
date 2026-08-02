#pragma once

#include "defines.h"
#include "core/io/stream/istream.h"
#include "core/io/serializer/serializer.h"
#include "platform/console.h"

class ConsoleInputStream : public IStream {
public:
    u64 read(void* buf, u64 size) override;
    b32 is_open() const override;
    u64 size() const override;
    u64 position() const override;
    b32 seek(u64 offset, FileSeekOrigin origin = FileSeekOrigin::Current) override;
    void close() override;

    template<typename T>
    ConsoleInputStream& operator>>(T& val) {
        deserialize(*this, val);
        return *this;
    }
};
