#pragma once

// compiler detection
#if defined(_MSC_VER)
#define SV_COMPILER_MSVC
#elif defined(__clang__)
#define SV_COMPILER_CLANG
#elif defined(__GNUC__)
#define SV_COMPILER_GCC
#endif

// cross-platform compatibility
#if defined(_MSC_VER)
#define SV_RESTRICT __restrict
#define SV_ALIGN(x) __declspec(align(x))
#include <intrin.h>
#define SV_BREAK() __debugbreak()
#define SV_FORCE_INLINE                                                        \
  __forceinline // only for little functions used extremely much
#define SV_INLINE inline
#define SV_COMPILER_BARRIER() _ReadWriteBarrier()
#define SV_CPU_BARRIER() MemoryBarrier()
#define SV_CTZ32(val) _tzcnt_u32(val)
#define SV_CTZ64(val) _tzcnt_u64(val)
#elif defined(__clang__)
#define SV_RESTRICT __restrict__
#define SV_ALIGN(x) __attribute__((aligned(x)))
#define SV_BREAK() __builtin_debugtrap()
#define SV_FORCE_INLINE                                                        \
  inline __attribute__((                                                       \
      always_inline)) // only for little functions used extremely much
#define SV_INLINE inline
#define SV_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
#define SV_CPU_BARRIER() __atomic_thread_fence(__ATOMIC_SEQ_CST)
#define SV_CTZ32(val) __builtin_ctz(val)
#define SV_CTZ64(val) __builtin_ctz(val)
#elif defined(__GNUC__)
#define SV_RESTRICT __restrict__
#define SV_ALIGN(x) __attribute__((aligned(x)))
#define SV_BREAK() __builtin_trap()
#define SV_FORCE_INLINE                                                        \
  inline __attribute__((                                                       \
      always_inline)) // only for little functions used extremely much
#define SV_INLINE inline
#define SV_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
#define SV_CPU_BARRIER() __atomic_thread_fence(__ATOMIC_SEQ_CST)
#define SV_CTZ32(val) __builtin_ctz(val)
#define SV_CTZ64(val) __builtin_ctz(val)
#else
#define SV_RESTRICT
#define SV_ALIGN(x) (void(0))
#define SV_BREAK() (void(0))
#define SV_INLINE
#define SV_STATIC_ASSERT
#define SV_ASSERT(cond, fmt, ...) (void(0))
#define SV_COMPILER_BARRIER() (void(0))
#define SV_CPU_BARRIER() (void(0))
#define SV_CTZ32(val) (void(0))
#define SV_CTZ64(val) (void(0))
#endif

#define SV_MEM_BARRIER()                                                       \
  do {                                                                         \
    SV_COMPILER_BARRIER();                                                     \
    SV_CPU_BARRIER();                                                          \
  } while (0)
