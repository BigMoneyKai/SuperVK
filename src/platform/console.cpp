#include "console.h"
#include "core/define/platform.h"

namespace Platform {

#if defined(SV_PLATFORM_LINUX)
#include <unistd.h>
#include <sys/types.h>
#elif defined(SV_PLATFORM_APPLE)
#include <unistd.h>
#include <sys/_types/_ssize_t.h>
#elif defined(SV_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

#if defined(SV_PLATFORM_LINUX) || defined(SV_PLATFORM_APPLE)
u64 console_read(void *buffer, u64 size) {
  ssize_t ret = read(STDIN_FILENO, buffer, size);

  if (ret < 0)
    return 0;

  return static_cast<u64>(ret);
}
u64 console_write(const void *buffer, u64 size) {
  ssize_t ret = write(STDOUT_FILENO, buffer, size);

  if (ret < 0)
    return 0;

  return static_cast<u64>(ret);
}

#elif defined(SV_PLATFORM_WINDOWS)
u64 console_read(void *buffer, u64 size) {
  DWORD readBytes = 0;

  ReadFile(GetStdHandle(STD_INPUT_HANDLE), buffer, static_cast<DWORD>(size),
           &readBytes, nullptr);

  return static_cast<u64>(readBytes);
}
u64 console_write(const void *buffer, u64 size) {
  DWORD writeBytes = 0;

  WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, static_cast<DWORD>(size),
            &writeBytes, nullptr);

  return static_cast<u64>(writeBytes);
}
#endif

} // namespace Platform
