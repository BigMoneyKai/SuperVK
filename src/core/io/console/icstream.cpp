#include "icstream.h"

u64 ConsoleInputStream::read(void* buf, u64 size) {
    return console_read(buf, size);
}

b32 ConsoleInputStream::is_open() const {
    return SV_TRUE;
}

u64 ConsoleInputStream::size() const {
    return 0;
}

u64 ConsoleInputStream::position() const {
    return 0;
}

b32 ConsoleInputStream::seek(u64 offset, FileSeekOrigin origin) {
    (void)offset;
    (void)origin;
    return SV_FALSE;
}

void ConsoleInputStream::close() {
}
