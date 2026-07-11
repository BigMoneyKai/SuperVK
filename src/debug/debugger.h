#pragma once

#include "utils/utils.h"

#include <format>
#include <print>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define COLOR_RESET     ""
#define COLOR_ORANGE    ""
#define COLOR_GREEN     ""
#define COLOR_PURPLE    ""
#define COLOR_RED       ""
#define COLOR_DARK_RED  ""
#define EMOJI_SMILE     ""
#define EMOJI_WARNING   ""
#define EMOJI_ERROR     ""
#define EMOJI_FATAL     ""
#define EMOJI_OK        ""
#define EMOJI_DEBUG     ""
#define EMOJI_LEXER     ""
#define EMOJI_PARSER    ""
#define EMOJI_AST       ""
#define EMOJI_IR        ""
#define EMOJI_CODEGEN   ""
#define EMOJI_LINKER    ""
#define EMOJI_RUN       ""
#define EMOJI_DEVIL     ""
#define EMOJI_ALERT     ""
#else
#define COLOR_RESET     "\033[0m"
#define COLOR_ORANGE    "\033[38;5;208m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_PURPLE    "\033[35m"
#define COLOR_RED       "\033[91m"
#define COLOR_DARK_RED  "\033[31m"
#define EMOJI_SMILE     "😄"
#define EMOJI_WARNING   "⚠️"
#define EMOJI_ERROR     "❌"
#define EMOJI_FATAL     "💥"
#define EMOJI_OK        "✅"
#define EMOJI_DEBUG     "🐛"
#define EMOJI_LEXER     "🔤"
#define EMOJI_PARSER    "🧩"
#define EMOJI_AST       "🌳"
#define EMOJI_IR        "🧠"
#define EMOJI_CODEGEN   "⚙️"
#define EMOJI_LINKER    "🔗"
#define EMOJI_RUN       "🚀"
#define EMOJI_DEVIL     "😈"
#define EMOJI_ALERT     "🚨"
#endif

typedef enum {
    DL_TRACE,
    DL_DEBUG,
    DL_INFO,
    DL_WARNING,
    DL_ERROR,
    DL_FATAL,
} DebugLevel;

// =========================================================================
// C++23 std::print backend — platform-portable, no %lu/%llu issues
// =========================================================================

#ifdef TRACE_MODE
#define TRACE(fmt, ...) \
    do { \
        std::print("{} {}[TRACE] in \"{}\" {}:{} [TRACE] {}\n", \
            EMOJI_OK, COLOR_GREEN, __func__, __FILE__, __LINE__, COLOR_PURPLE, \
            std::format(fmt __VA_OPT__(,) __VA_ARGS__)); \
    } while(0)
#else
#define TRACE(...) ((void)0)
#endif

#ifndef NDEBUG
#define DEBUG(fmt, ...) \
    do { \
        std::print("{}{} {}[DEBUG]{} {}\n", \
            EMOJI_RUN, EMOJI_CODEGEN, COLOR_ORANGE, COLOR_RESET, \
            std::format(fmt __VA_OPT__(,) __VA_ARGS__)); \
    } while(0)
#else
#define DEBUG(...) ((void)0)
#endif

#define INFO(fmt, ...) \
    do { \
        std::print("{}{} {}[INFO]{} {}\n", \
            EMOJI_OK, EMOJI_SMILE, COLOR_GREEN, COLOR_RESET, \
            std::format(fmt __VA_OPT__(,) __VA_ARGS__)); \
    } while(0)

#define WARNING(fmt, ...) \
    do { \
        std::print(stderr, "{}{} in \"{}\" {}:{} {}[WARNING]{} {}\n", \
            EMOJI_DEVIL, EMOJI_WARNING, __func__, __FILE__, __LINE__, \
            COLOR_PURPLE, COLOR_RESET, \
            std::format(fmt __VA_OPT__(,) __VA_ARGS__)); \
    } while(0)

#define ERROR(fmt, ...) \
    do { \
        std::print(stderr, "{}{} in \"{}\" {}:{} {}[ERROR]{} {}\n", \
            EMOJI_ERROR, EMOJI_DEBUG, __func__, __FILE__, __LINE__, \
            COLOR_RED, COLOR_RESET, \
            std::format(fmt __VA_OPT__(,) __VA_ARGS__)); \
    } while(0)

#define FATAL(fmt, ...) \
    do { \
        std::print(stderr, "{}{} in \"{}\" {}:{} {}[FATAL]{} {}\n", \
            EMOJI_ALERT, EMOJI_FATAL, __func__, __FILE__, __LINE__, \
            COLOR_DARK_RED, COLOR_RESET, \
            std::format(fmt __VA_OPT__(,) __VA_ARGS__)); \
        failure_exit(); \
    } while(0)

#ifndef NDEBUG
#define VK_CHECK_RESULT(func)\
    do { \
        VkResult result = func; \
        if(result != VK_SUCCESS) { \
            FATAL("Failed to load " #func); \
        } \
        TRACE("Success to load " #func); \
    } while(0)
#else
#define VK_CHECK_RESULT(func) (func)
#endif

#define VAL_NAME(val) #val
