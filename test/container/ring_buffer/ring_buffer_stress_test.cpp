#include <gtest/gtest.h>

#include "core/container/ring_buffer/ring_buffer.hpp"
#include "core/container/string/string.hpp"

// ============================================================================
// Large-scale interleaved push / pop: forces repeated wrap-around + growth
// ============================================================================

TEST(RingBufferStressTest, InterleavedPushPopMillion) {
  RingBuffer<u64> rb;
  constexpr u64 N = 1'000'000;
  u64 nextPush = 0;
  u64 nextPop = 0;

  for (u64 i = 0; i < N; ++i) {
    rb.push_back(nextPush++);
    if (i % 3 == 0) { // consume periodically to force wrap-around and growth
      ASSERT_FALSE(rb.empty());
      EXPECT_EQ(rb.front(), nextPop);
      rb.pop_front();
      ++nextPop;
    }
  }

  // remaining elements must be strictly ordered
  u64 expect = nextPop;
  for (auto v : rb) {
    EXPECT_EQ(v, expect);
    ++expect;
  }
  EXPECT_EQ(expect, nextPush);
  EXPECT_EQ(rb.size(), nextPush - nextPop);
}

TEST(RingBufferStressTest, WrapAndGrowPreservesOrder) {
  RingBuffer<u64> rb(64, 0); // capacity constructor: all 64 elements are 0

  // pop 10 -> head is no longer 0, creating a wrap-around state
  for (int i = 0; i < 10; ++i) {
    rb.pop_front();
  }
  EXPECT_EQ(rb.size(), 54);

  // push 1000 -> wraps around and grows multiple times
  constexpr u64 PUSH = 1'000;
  for (u64 i = 1; i <= PUSH; ++i) {
    rb.push_back(i);
  }
  EXPECT_EQ(rb.size(), 54 + PUSH);

  // order: remaining 54 zeros, then 1..1000
  u64 zeroSeen = 0;
  u64 pushExpect = 1;
  for (auto v : rb) {
    if (zeroSeen < 54) {
      EXPECT_EQ(v, 0);
      ++zeroSeen;
    } else {
      EXPECT_EQ(v, pushExpect);
      ++pushExpect;
    }
  }
  EXPECT_EQ(zeroSeen, 54);
  EXPECT_EQ(pushExpect, PUSH + 1);
}

// ============================================================================
// Large-scale insert / erase / pop
// ============================================================================

TEST(RingBufferStressTest, InsertEraseChurn) {
  RingBuffer<u64> rb;
  constexpr u64 N = 5'000;
  for (u64 i = 0; i < N; ++i) {
    rb.push_back(i);
  }

  for (u64 k = 0; k < 500; ++k) {
    rb.insert(rb.size() / 2, 999'999);
  }
  EXPECT_EQ(rb.size(), N + 500);

  for (u64 k = 0; k < 500; ++k) {
    rb.erase(rb.size() / 2);
  }
  EXPECT_EQ(rb.size(), N);
  EXPECT_EQ(rb.front(), 0);
  EXPECT_EQ(rb.back(), N - 1);

  for (u64 k = 0; k < 100; ++k) {
    rb.pop_front();
  }
  EXPECT_EQ(rb.size(), N - 100);
  EXPECT_EQ(rb.front(), 100);

  rb.clear();
  EXPECT_TRUE(rb.empty());
}

// ============================================================================
// Large-scale non-trivial elements
// ============================================================================

TEST(RingBufferStressTest, StringHeavy) {
  RingBuffer<String> rb;
  const char *s = "0123456789abcdefghijklmnopqrstuvwxyzABCDEF";
  constexpr u64 N = 50'000;

  for (u64 i = 0; i < N; ++i) {
    rb.push_back(String(s));
  }
  ASSERT_EQ(rb.size(), N);
  EXPECT_EQ(rb.front(), String(s));
  EXPECT_EQ(rb.back(), String(s));

  for (u64 i = 0; i < N / 2; ++i) {
    rb.pop_front();
  }
  EXPECT_EQ(rb.size(), N / 2);
  EXPECT_EQ(rb.front(), String(s));

  rb.clear();
  EXPECT_TRUE(rb.empty());
}
