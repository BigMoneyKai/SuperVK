#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"

// fingerprint: top 7 bits of hash, with high bit set (non-zero = occupied)
// empty slot: 0x00
// group size: 16

static constexpr u64 GROUP_SHIFT = 4;
static constexpr u64 GROUP_SIZE = 16;
static constexpr u64 GROUP_MASK = 15;

SV_FORCE_INLINE u8 make_fingerprint(u64 hash) {
  return static_cast<u8>((hash >> 57) | 0x80);
}

// =========================================================================
// SIMD: broadcast + cmpeq + movemask → 16-bit bitmask of matches
// =========================================================================

#if defined(__x86_64__) || defined(_M_X64)
#include <emmintrin.h> // SSE2
#include <tmmintrin.h> // SSSE3

SV_FORCE_INLINE u32 match_group(const u8 *fps, u8 target) {
  __m128i v = _mm_loadu_si128(reinterpret_cast<const __m128i *>(fps));
  __m128i t = _mm_set1_epi8(static_cast<char>(target));
  __m128i c = _mm_cmpeq_epi8(v, t);
  return static_cast<u32>(_mm_movemask_epi8(c));
}

#elif defined(__aarch64__) || defined(_M_ARM64)
#include <arm_neon.h>

SV_FORCE_INLINE u32 match_group(const u8 *fps, u8 target) {
  uint8x16_t v = vld1q_u8(fps);
  uint8x16_t t = vdupq_n_u8(target);
  uint8x16_t c = vceqq_u8(v, t);
  // NEON has no native movemask — store to stack, extract bits
  alignas(16) u8 buf[16];
  vst1q_u8(buf, c);
  u32 mask = 0;
  for (int i = 0; i < 16; ++i)
    mask |= (buf[i] ? 1u : 0u) << i;
  return mask;
}

#else
// fallback: scalar loop over 16 bytes
SV_FORCE_INLINE u32 match_group(const u8 *fps, u8 target) {
  u32 mask = 0;
  for (u64 i = 0; i < GROUP_SIZE; ++i)
    if (fps[i] == target)
      mask |= (1u << i);
  return mask;
}
#endif
