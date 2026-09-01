#pragma once

#include "assert.h"

#include <cstdint>

// unsigned int
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

// signed int
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

// float point types
using f32 = float;
using f64 = double;

// boolean types
using b32 = std::int32_t;
using b8 = std::int8_t;

// address types
using addr = std::uintptr_t;

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
