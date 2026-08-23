#pragma once

#include <iostream>
#include <format>

#if defined(_MSC_VER)
#define SV_STATIC_ASSERT static_assert
#define SV_ASSERT(cond, fmt, ...)                                              \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::cerr << "Abort: [" << std::format(fmt, __VA_ARGS__)                 \
                << "] at " << __FILE__ << ":" << __LINE__ << std::endl;        \
      abort();                                                                 \
    }                                                                          \
  } while (0)
#elif defined(__clang__)
#define SV_STATIC_ASSERT static_assert
#define SV_ASSERT(cond, fmt, ...)                                              \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::cerr << "Abort: [" << std::format(fmt __VA_OPT__(, ) __VA_ARGS__)   \
                << "] at " << __FILE__ << ":" << __LINE__ << std::endl;        \
      abort();                                                                 \
    }                                                                          \
  } while (0)
#elif defined(__GNUC__)
#define SV_STATIC_ASSERT static_assert
#define SV_ASSERT(cond, fmt, ...)                                              \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::cerr << "Abort: [" << std::format(fmt __VA_OPT__(, ) __VA_ARGS__)   \
                << "] at " << __FILE__ << ":" << __LINE__ << std::endl;        \
      abort();                                                                 \
    }                                                                          \
  } while (0)
#endif
