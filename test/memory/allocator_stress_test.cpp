#include <gtest/gtest.h>

#include "core/memory/heap_allocator.h"
#include "core/memory/linear_allocator.h"
#include "core/memory/pool_allocator.h"
#include "core/memory/stack_allocator.h"

#include <cstring>
#include <random>

// ============================================================================
// HeapAllocator: large-scale allocate / deallocate
// ============================================================================

TEST(AllocatorStressTest, HeapChurn) {
  HeapAllocator &heap = HeapAllocator::instance();
  const u64 usedBefore = heap.usedSize();
  std::mt19937 rng(777);
  constexpr u64 N = 100'000;

  for (u64 i = 0; i < N; ++i) {
    const u64 size = 1 + (rng() % 2048);
    void *p = heap.allocate(size, 16);
    ASSERT_NE(p, nullptr);
    std::memset(p, static_cast<i32>(i & 0xFF), size);
    heap.deallocate(p);
  }

  EXPECT_EQ(heap.usedSize(), usedBefore);
}

// ============================================================================
// PoolAllocator: exhaust all chunks -> free in random order -> reusable
// ============================================================================

TEST(AllocatorStressTest, PoolExhaustReuse) {
  PoolAllocator pool(64, 4096, 16);
  constexpr u64 N = 4096;
  void *blocks[N];

  for (u64 i = 0; i < N; ++i) {
    blocks[i] = pool.allocate(64, 16);
    ASSERT_NE(blocks[i], nullptr);
    std::memset(blocks[i], static_cast<i32>(i & 0xFF), 64);
  }
  EXPECT_EQ(pool.allocate(64, 16), nullptr); // pool exhausted

  // shuffle then free in random order
  std::mt19937 rng(42);
  for (u64 i = 0; i < N; ++i) {
    const u64 j = rng() % (i + 1);
    std::swap(blocks[i], blocks[j]);
  }
  for (u64 i = 0; i < N; ++i) {
    pool.deallocate(blocks[i]);
  }

  // reusable after everything is freed
  void *p = pool.allocate(64, 16);
  ASSERT_NE(p, nullptr);
  pool.deallocate(p);
}

// ============================================================================
// StackAllocator: large-scale push / LIFO free / marker rollback
// ============================================================================

TEST(AllocatorStressTest, StackPushPopLifo) {
  StackAllocator stack(1 << 20); // 1MB
  std::mt19937 rng(7);
  constexpr u64 N = 5'000;
  void *blocks[N];
  u64 count = 0;

  for (u64 i = 0; i < N; ++i) {
    const u64 size = 1 + (rng() % 512);
    void *p = stack.allocate(size, 16);
    if (!p) {
      break; // stack full
    }
    blocks[count++] = p;
    std::memset(p, static_cast<i32>(i & 0xFF), size);
  }
  EXPECT_GT(count, 1000);

  while (count > 0) {
    stack.deallocate(blocks[--count]); // LIFO
  }
  EXPECT_EQ(stack.used(), 0);

  // marker + reset_to reuse
  void *a = stack.allocate(128, 16);
  Marker marker{stack.used()};
  void *b = stack.allocate(256, 16);
  EXPECT_NE(a, b);
  stack.reset_to(marker);
  void *c = stack.allocate(256, 16);
  EXPECT_EQ(b, c);

  stack.reset();
  EXPECT_EQ(stack.used(), 0);
}

// ============================================================================
// LinearAllocator: fill up -> exhausted -> reset reuse
// ============================================================================

TEST(AllocatorStressTest, LinearAllocateUntilFull) {
  LinearAllocator linear(1 << 20);
  std::mt19937 rng(99);
  u64 count = 0;

  while (true) {
    const u64 size = 1 + (rng() % 4096);
    void *p = linear.allocate(size, 16);
    if (!p) {
      break;
    }
    std::memset(p, 0xAB, size);
    ++count;
  }
  EXPECT_GT(count, 100);

  // the last large request not fitting does not mean exhaustion: keep filling
  // with small chunks until truly full
  while (linear.allocate(16, 16) != nullptr) {
  }
  EXPECT_EQ(linear.allocate(16, 16), nullptr);

  linear.reset();
  EXPECT_EQ(linear.used(), 0);

  void *a = linear.allocate(64, 16);
  ASSERT_NE(a, nullptr);
  linear.reset();
  void *b = linear.allocate(64, 16);
  EXPECT_EQ(a, b); // same memory reused after reset
}
