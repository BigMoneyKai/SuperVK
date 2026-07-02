#pragma once

#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define COLOR_RESET     ""
#define COLOR_ORANGE    ""
#define COLOR_GREEN     ""
#define COLOR_PURPLE    ""
#define COLOR_RED       ""
#define COLOR_DARK_RED  ""
#define EMOJI_SMILE     "😄"
#define EMOJI_WARNING   "⚠️"
#define EMOJI_ERROR     "X"
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

// NOTE: Trace mode - show trace/debug/info/warning/error/fatal
// NOTE: Debug mode - show debug/info/warning/error/fatal
// NOTE: Release mode - info/warning/error/fatal

typedef enum {
    DL_TRACE,
    DL_DEBUG,

    DL_INFO,
    DL_WARNING,
    DL_ERROR,
    DL_FATAL,

} DebugLevel;

#ifdef TRACE_MODE
#define TRACE(...) \
    do{\
        fprintf(stdout, EMOJI_OK EMOJI_SMILE " " COLOR_GREEN "TRACE" COLOR_RESET": ");\
        fprintf(stderr, EMOJI_DEVIL EMOJI_WARNING " in \"%s\" %s:%d " COLOR_PURPLE "[TRACE]" COLOR_RESET " ", __func__, __FILE__, __LINE__);\
        fprintf(stdout, __VA_ARGS__);\
        fprintf(stdout, "\n");\
    } while(0)
#else
#define TRACE(...) ((void)0)
#endif

#ifndef NDEBUG
#define DEBUG(...) \
    do{\
        fprintf(stdout, EMOJI_RUN EMOJI_CODEGEN " " COLOR_ORANGE "[DEBUG]" COLOR_RESET " ");\
        fprintf(stdout, __VA_ARGS__);\
        fprintf(stdout, "\n");\
    } while(0)
#else
#define DEBUG(...) ((void)0)
#endif

#define INFO(...) \
    do{\
        fprintf(stdout, EMOJI_OK EMOJI_SMILE " " COLOR_GREEN "[INFO]" COLOR_RESET " ");\
        fprintf(stdout, __VA_ARGS__);\
        fprintf(stdout, "\n");\
    } while(0)

#define WARNING(...) \
    do{\
        fprintf(stderr, EMOJI_DEVIL EMOJI_WARNING " in \"%s\" %s:%d " COLOR_PURPLE "[WARNING]" COLOR_RESET " ", __func__, __FILE__, __LINE__);\
        fprintf(stderr, __VA_ARGS__);\
        fprintf(stderr, "\n");\
    } while(0)

#define ERROR(...) \
    do{\
        fprintf(stderr, EMOJI_ERROR EMOJI_DEBUG " in \"%s\" %s:%d " COLOR_RED "[ERROR]" COLOR_RESET " ", __func__, __FILE__, __LINE__);\
        fprintf(stderr, __VA_ARGS__);\
        fprintf(stderr, "\n");\
    } while(0)

#define FATAL(...) \
    do{\
        fprintf(stderr, EMOJI_ALERT EMOJI_FATAL " in \"%s\" %s:%d " COLOR_DARK_RED "[FATAL]" COLOR_RESET " ", __func__, __FILE__, __LINE__);\
        fprintf(stderr, __VA_ARGS__);\
        fprintf(stderr, "\n");\
        failure_exit();\
    } while(0)

#ifndef NDEBUG
#define VK_CHECK_RESULT(func)\
    do {\
        VkResult result = func;\
        if(result != VK_SUCCESS) {\
            FATAL(#func" failure");\
        }\
        TRACE(#func" success");\
    } while(0)
#else
#define VK_CHECK_RESULT(func) (func)
#endif
