#pragma once

#include "log_tag.h"
#include "logger.h"
#include "logger_attrib.h"
#include "platform/time.h"
#include "utils/utils.h"

#include <format>
#include <print>
#include <stdlib.h>

#ifdef TRACE_MODE
#define TRACE(catag, fmt, ...)                                                 \
  do {                                                                         \
    LogMsg msg{                                                                \
        LogLevel::Trace, catag,                                                \
        Logger::id(),    std::format(fmt __VA_OPT__(, ) __VA_ARGS__).c_str(),  \
        __func__,        __FILE__,                                             \
        __LINE__,        static_cast<u64>(timer_now_ms() * 1000.0)};           \
    Logger::submit(std::move(msg));                                            \
  } while (0)
#else
#define TRACE(catag, fmt, ...) ((void)0)
#endif

#ifndef NDEBUG
#define DEBUG(catag, fmt, ...)                                                 \
  do {                                                                         \
    LogMsg msg{                                                                \
        LogLevel::Debug, catag,                                                \
        Logger::id(),    std::format(fmt __VA_OPT__(, ) __VA_ARGS__).c_str(),  \
        __func__,        __FILE__,                                             \
        __LINE__,        static_cast<u64>(timer_now_ms() * 1000.0)};           \
    Logger::submit(std::move(msg));                                            \
  } while (0)
#else
#define DEBUG(catag, fmt, ...) ((void)0)
#endif

#define INFO(catag, fmt, ...)                                                  \
  do {                                                                         \
    LogMsg msg{                                                                \
        LogLevel::Info, catag,                                                 \
        Logger::id(),   std::format(fmt __VA_OPT__(, ) __VA_ARGS__).c_str(),   \
        __func__,       __FILE__,                                              \
        __LINE__,       static_cast<u64>(timer_now_ms() * 1000.0)};            \
    Logger::submit(std::move(msg));                                            \
  } while (0)

#define WARNING(catag, fmt, ...)                                               \
  do {                                                                         \
    LogMsg msg{LogLevel::Warning,                                              \
               catag,                                                          \
               Logger::id(),                                                   \
               std::format(fmt __VA_OPT__(, ) __VA_ARGS__).c_str(),            \
               __func__,                                                       \
               __FILE__,                                                       \
               __LINE__,                                                       \
               static_cast<u64>(timer_now_ms() * 1000.0)};                     \
    Logger::submit(std::move(msg));                                            \
  } while (0)

#define ERROR(catag, fmt, ...)                                                 \
  do {                                                                         \
    LogMsg msg{                                                                \
        LogLevel::Error, catag,                                                \
        Logger::id(),    std::format(fmt __VA_OPT__(, ) __VA_ARGS__).c_str(),  \
        __func__,        __FILE__,                                             \
        __LINE__,        static_cast<u64>(timer_now_ms() * 1000.0)};           \
    Logger::submit(std::move(msg));                                            \
  } while (0)

#define FATAL(catag, fmt, ...)                                                 \
  do {                                                                         \
    LogMsg msg{                                                                \
        LogLevel::Fatal, catag,                                                \
        Logger::id(),    std::format(fmt __VA_OPT__(, ) __VA_ARGS__).c_str(),  \
        __func__,        __FILE__,                                             \
        __LINE__,        static_cast<u64>(timer_now_ms() * 1000.0)};           \
    Logger::submit(std::move(msg));                                            \
    failure_exit();                                                            \
  } while (0)
// #define INFO(catag, fmt, ...)   (void(0)) \
// #define WARNING(catag, fmt, ...) (void(0)) \
// #define ERROR(catag, fmt, ...)   (void(0)) \
// #define FATAL(catag, fmt, ...)   failure_exit(); \

// Vulkan debug
#ifndef NDEBUG
#define VK_CHECK_RESULT(expr)                                                  \
  do {                                                                         \
    VkResult result = (expr);                                                  \
    if (result != VK_SUCCESS) {                                                \
      LogMsg msg{.level = LogLevel::Fatal,                                     \
                 .catag = LogCatag::Vulkan,                                    \
                 .id = Logger::id(),                                           \
                 .msg = "Vulkan error: ",                                      \
                 .func = __func__,                                             \
                 .file = __FILE__,                                             \
                 .line = __LINE__,                                             \
                 .timestamp = static_cast<u64>(timer_now_ms() * 1000.0)};      \
      Logger::submit(std::move(msg));                                          \
    }                                                                          \
  } while (0)
#else
#define VK_CHECK_RESULT(func) (func)
#endif

#define VAL_NAME(val) #val
