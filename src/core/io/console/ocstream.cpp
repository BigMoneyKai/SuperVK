#include "ocstream.h"
#include "platform/console.h"

b32 ConsoleOutputStream::write(const void *buf, u64 size) {
  return console_write(buf, size) == size ? SV_TRUE : SV_FALSE;
}

b32 ConsoleOutputStream::is_open() const { return SV_TRUE; }

u64 ConsoleOutputStream::size() const { return 0; }

u64 ConsoleOutputStream::position() const { return 0; }

b32 ConsoleOutputStream::seek(u64 offset, FileSeekOrigin origin) {
  (void)offset;
  (void)origin;
  return SV_FALSE;
}

void ConsoleOutputStream::close() {}
