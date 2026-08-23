#pragma once

#include <cstdlib>
#include <format>
#include <iostream>
#include <utility>

namespace sv_assert_detail {
template <typename... Args>
[[noreturn]] inline void assert_fail(const char *file, int line,
                                     std::format_string<Args...> fmt,
                                     Args &&...args) {
  std::cerr << "Abort: [" << std::format(fmt, std::forward<Args>(args)...)
            << "] at " << file << ":" << line << std::endl;
  abort();
}
} // namespace sv_assert_detail

#define SV_STATIC_ASSERT static_assert

#define SV_ASSERT(cond, ...)                                                   \
  do {                                                                         \
    if (!(cond)) {                                                             \
      sv_assert_detail::assert_fail(__FILE__, __LINE__, __VA_ARGS__);          \
    }                                                                          \
  } while (0)
