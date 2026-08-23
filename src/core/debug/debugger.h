#pragma once

#include "log_tag.h"
#include "logger.h"
#include "logger_attrib.h"
#include "platform/time.h"
#include "utils/utils.h"

#include <format>
#include <stdlib.h>
#include <utility>

namespace sv_log_detail {
template <typename... Args>
inline void log_emit(LogLevel level, LogCatag catag, const char *func,
                     const char *file, u32 line,
                     std::format_string<Args...> fmt, Args &&...args) {
  LogMsg msg{
      level,        catag,
      Logger::id(), std::format(fmt, std::forward<Args>(args)...).c_str(),
      func,         file,
      line,         static_cast<u64>(timer_now_ms() * 1000.0)};
  Logger::submit(std::move(msg));
}
} // namespace sv_log_detail

#ifdef TRACE_MODE
#define TRACE(catag, ...)                                                      \
  do {                                                                         \
    sv_log_detail::log_emit(LogLevel::trace, catag, __func__, __FILE__,        \
                            __LINE__, __VA_ARGS__);                            \
  } while (0)
#else
#define TRACE(catag, ...) ((void)0)
#endif

#ifndef NDEBUG
#define DEBUG(catag, ...)                                                      \
  do {                                                                         \
    sv_log_detail::log_emit(LogLevel::debug, catag, __func__, __FILE__,        \
                            __LINE__, __VA_ARGS__);                            \
  } while (0)
#else
#define DEBUG(catag, ...) ((void)0)
#endif

#define INFO(catag, ...)                                                       \
  do {                                                                         \
    sv_log_detail::log_emit(LogLevel::info, catag, __func__, __FILE__,         \
                            __LINE__, __VA_ARGS__);                            \
  } while (0)

#define WARNING(catag, ...)                                                    \
  do {                                                                         \
    sv_log_detail::log_emit(LogLevel::warning, catag, __func__, __FILE__,      \
                            __LINE__, __VA_ARGS__);                            \
  } while (0)

#define ERROR(catag, ...)                                                      \
  do {                                                                         \
    sv_log_detail::log_emit(LogLevel::error, catag, __func__, __FILE__,        \
                            __LINE__, __VA_ARGS__);                            \
  } while (0)

#define FATAL(catag, ...)                                                      \
  do {                                                                         \
    sv_log_detail::log_emit(LogLevel::fatal, catag, __func__, __FILE__,        \
                            __LINE__, __VA_ARGS__);                            \
    Logger::print_all();                                                       \
    failure_exit();                                                            \
  } while (0)
// #define INFO(catag, ...)   (void(0)) \
// #define WARNING(catag, ...) (void(0)) \
// #define ERROR(catag, ...)   (void(0)) \
// #define FATAL(catag, ...)   failure_exit(); \

// Vulkan debug
#ifndef NDEBUG
#define VK_CHECK_RESULT(expr)                                                  \
  do {                                                                         \
    VkResult result = (expr);                                                  \
    if (result != VK_SUCCESS) {                                                \
      LogMsg msg{.level = LogLevel::fatal,                                     \
                 .catag = LogCatag::vulkan,                                    \
                 .id = Logger::id(),                                           \
                 .msg = "Vulkan error: ",                                      \
                 .func = __func__,                                             \
                 .file = __FILE__,                                             \
                 .line = __LINE__,                                             \
                 .timestamp = static_cast<u64>(timer_now_ms() * 1000.0)};      \
      Logger::submit(std::move(msg));                                          \
      Logger::print_all();                                                     \
      failure_exit();                                                          \
    }                                                                          \
  } while (0)
#else
#define VK_CHECK_RESULT(func) (func)
#endif

#define VAL_NAME(val) #val
