#pragma once

#include <cstdint>
#include <iostream>

// platform compatibility
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define SV_PLATFORM_WINDOWS 1
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
        #error "Other unsupported operating system."
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

// compiler detection
#if defined(_MSC_VER)
    #define SV_COMPILER_MSVC 1
#elif defined(__clang__)
    #define SV_COMPILER_CLANG 1
#elif defined(__GNUC__)
    #define SV_COMPILER_GCC 1
#endif

// cross-platform compatibility
#if defined(_MSC_VER)
    #define SV_RESTRICT __restrict
    #define SV_ALIGN(x) __declspec(align(x))
    #include <intrin.h>
    #define SV_BREAK() __debugbreak()
    #define SV_FORCE_INLINE __forceinline // only for little functions used extremely much
    #define SV_INLINE inline
    #define SV_STATIC_ASSERT static_assert
    #define SV_ASSERT(cond, fmt, ...)\
        do {\
            if(!(cond)) {\
                std::cerr\
                    << "Abort: ["\
                    << std::format(fmt __VA_OPT__(,) __VA_ARGS__)\
                    << "] at "\
                    << __FILE__ << ":" << __LINE__\
                    << std::endl;\
                abort();\
            }\
        } while(0)
    #define SV_COMPILER_BARRIER() _ReadWriteBarrier()
    #define SV_CPU_BARRIER() MemoryBarrier()
#elif defined(__GNUC__)
    #define SV_RESTRICT __restrict__
    #define SV_ALIGN(x) __attribute__((aligned(x)))
    #define SV_BREAK() __builtin_trap()
    #define SV_FORCE_INLINE inline __attribute__((always_inline)) // only for little functions used extremely much
    #define SV_INLINE inline
    #define SV_STATIC_ASSERT static_assert
    #define SV_ASSERT(cond, fmt, ...)\
        do {\
            if(!(cond)) {\
                std::cerr\
                    << "Abort: ["\
                    << std::format(fmt __VA_OPT__(,) __VA_ARGS__)\
                    << "] at "\
                    << __FILE__ << ":" << __LINE__\
                    << std::endl;\
                abort();\
            }\
        } while(0)
    #define SV_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
    #define SV_CPU_BARRIER() __atomic_thread_fence(__ATOMIC_SEQ_CST)
#elif defined(__clang__)
    #define SV_RESTRICT __restrict__
    #define SV_ALIGN(x) __attribute__((aligned(x)))
    #define SV_BREAK() __builtin_debugtrap()
    #define SV_FORCE_INLINE inline __attribute__((always_inline)) // only for little functions used extremely much
    #define SV_INLINE inline
    #define SV_STATIC_ASSERT static_assert
    #define SV_ASSERT(cond, fmt, ...)\
        do {\
            if(!(cond)) {\
                std::cerr\
                    << "Abort: ["\
                    << std::format(fmt __VA_OPT__(,) __VA_ARGS__)\
                    << "] at "\
                    << __FILE__ << ":" << __LINE__\
                    << std::endl;\
                abort();\
            }\
        } while(0)
    #define SV_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
    #define SV_CPU_BARRIER() __atomic_thread_fence(__ATOMIC_SEQ_CST)
#else
    #define SV_RESTRICT
    #define SV_ALIGN(x) (void(0))
    #define SV_BREAK() (void(0))
    #define SV_INLINE
    #define SV_STATIC_ASSERT
    #define SV_ASSERT(cond, fmt, ...) (void(0))
    #define SV_COMPILER_BARRIER() (void(0))
    #define SV_CPU_BARRIER() (void(0))
#endif

#define SV_MEM_BARRIER() \
    do { SV_COMPILER_BARRIER(); SV_CPU_BARRIER(); } while(0)

// unsigned int
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// signed int
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// float point types
typedef float f32;
typedef double f64;

// boolean types
typedef int32_t b32;
typedef int8_t b8;

// address types
typedef uintptr_t addr;

// type assertions
SV_STATIC_ASSERT(sizeof(u8) == 1, "Exptypeofected u8 to be 1 byte.");
SV_STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 byte.");
SV_STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 byte.");
SV_STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 byte.");

SV_STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
SV_STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 byte.");
SV_STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 byte.");
SV_STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 byte.");

SV_STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 byte.");
SV_STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 byte.");

SV_STATIC_ASSERT(sizeof(b32) == 4, "Expected b32 to be 4 byte.");
SV_STATIC_ASSERT(sizeof(b8) == 1, "Expected b8 to be 1 byte.");

SV_STATIC_ASSERT(sizeof(addr) == sizeof(void*), "Expected addr to match pointer size.");

// define boolean values
#define SV_TRUE 1
#define SV_FALSE 0

// .dll/.so
#if defined(_MSC_VER)
    #if defined(SV_EXPORT)
        #define SVAPI __declspec(dllexport)
    #elif defined(SV_IMPORT)
        #define SVAPI __declspec(dllimport)
    #else
        #define SVAPI
    #endif
#elif defined(__GNUC__) || defined(__clang__)
    #define SVAPI __attribute__((visibility("default")))
#else
    #define SVAPI
#endif

// API choice
#if defined(SV_PLATFORM_WINDOWS)
    #define SV_RENDER_API_D3D11
    #define SV_RENDER_API_VULKAN
#elif defined(SV_PLATFORM_APPLE)
    #define SV_RENDER_API_METAL
    #define SV_RENDER_API_VULKAN
#elif defined(SV_PLATFORM_LINUX)
    #define SV_RENDER_API_VULKAN
#else
    #define SV_RENDER_API_NO_SUPPORT
#endif

