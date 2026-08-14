#pragma once

// platform compatibility
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define SV_PLATFORM_WINDOWS
#ifndef _WIN64
#error "64-bit is required on Windows."
#endif

#elif defined(__APPLE__)
#define SV_PLATFORM_APPLE
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define SV_PLATFORM_IOS
#elif TARGET_OS_MAC
#define SV_PLATFORM_MAC
#else
#error "Unknown/Unsupported operating system."
#endif

#elif defined(__linux__) || defined(__gnu_linux__)
#define SV_PLATFORM_LINUX
#if defined(__ANDROID__)
#define SV_PLATFORM_ANDROID
#endif

#elif defined(__unix__)
#define SV_PLATFORM_UNIX
#if defined(_POSIX_VERSION)
#define SV_PLATFORM_POSIX
#endif

#else
#error "Unknown/Unsupported operating system."
#endif
