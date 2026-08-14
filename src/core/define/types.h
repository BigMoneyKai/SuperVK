#pragma once

#include "assert.h"

#include <cstdint>

// unsigned int
typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

// signed int
typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

// float point types
typedef float f32;
typedef double f64;

// boolean types
typedef std::int32_t b32;
typedef std::int8_t b8;

// address types
typedef std::uintptr_t addr;

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

SV_STATIC_ASSERT(sizeof(addr) == sizeof(void *),
                 "Expected addr to match pointer size.");

// define boolean values
#define SV_TRUE 1
#define SV_FALSE 0
