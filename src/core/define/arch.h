#pragma once

// Chip architecture
#if defined(__x86_64__) || defined(_M_X64)
#define SV_ARCH_X64

#elif defined(__aarch64__) || defined(_M_ARM64)
#define SV_ARCH_ARM64

#elif defined(__i386__) || defined(_M_IX86)
#define SV_ARCH_X86

#elif defined(__arm__) || defined(_M_ARM)
#define SV_ARCH_ARM

#elif defined(__riscv)
#define SV_ARCH_RISCV

#else
#define SV_ARCH_UNKNOWN
#error "Unsupported chip architecture"
#endif

#if defined(SV_ARCH_AARCH64)
#define SV_CACHE_LINE_SIZE 128
#else
#define SV_CACHE_LINE_SIZE 64
#endif

