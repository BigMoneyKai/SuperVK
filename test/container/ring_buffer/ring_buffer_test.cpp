#include <gtest/gtest.h>

#include "core/container/ring_buffer/ring_buffer.hpp"
#include "core/container/string/string.hpp"

// ============================================================================
// Construction / empty
// ============================================================================

TEST(RingBufferTest, DefaultEmpty) {
  RingBuffer<u64> rb;
  EXPECT_TRUE(rb.empty());
  EXPECT_EQ(rb.size(), 0);
  EXPECT_EQ(rb.capacity(), 0);
}

TEST(RingBufferTest, CapacityConstructor) {
  RingBuffer<u64> rb(4, 7);
  EXPECT_EQ(rb.size(), 4);
  EXPECT_GE(rb.capacity(), 4);
  for (u64 i = 0; i < rb.size(); ++i) {
    EXPECT_EQ(rb[i], 7);
  }
}

// ============================================================================
// push / pop / front / back
// ============================================================================

TEST(RingBufferTest, PushBackFrontBack) {
  RingBuffer<u64> rb;
  rb.push_back(10);
  rb.push_back(20);
  rb.push_back(30);

  EXPECT_EQ(rb.size(), 3);
  EXPECT_EQ(rb.front(), 10);
  EXPECT_EQ(rb.back(), 30);
}

TEST(RingBufferTest, PopFrontOrder) {
  RingBuffer<u64> rb;
  for (u64 i = 0; i < 4; ++i) {
    rb.push_back(i);
  }

  u64 expected = 0;
  while (!rb.empty()) {
    EXPECT_EQ(rb.front(), expected);
    ++expected;
    rb.pop_front();
  }
  EXPECT_TRUE(rb.empty());
}

TEST(RingBufferTest, WrapAroundOrder) {
  // fixed capacity + pop before push, forcing head away from 0 (wrap-around)
  RingBuffer<u64> rb(4, 0);
  rb.pop_front();
  rb.pop_front();
  rb.push_back(100);
  rb.push_back(200);

  EXPECT_EQ(rb.size(), 4);
  EXPECT_EQ(rb.front(), 0);
  EXPECT_EQ(rb.back(), 200);

  const u64 expected[] = {0, 0, 100, 200};
  u64 i = 0;
  for (auto v : rb) {
    EXPECT_EQ(v, expected[i]);
    ++i;
  }
}

TEST(RingBufferTest, GrowPreservesOrder) {
  RingBuffer<u64> rb;
  constexpr u64 N = 1000;
  for (u64 i = 0; i < N; ++i) {
    rb.push_back(i);
  }

  EXPECT_EQ(rb.size(), N);
  EXPECT_EQ(rb.front(), 0);
  EXPECT_EQ(rb.back(), N - 1);

  u64 i = 0;
  for (auto v : rb) {
    EXPECT_EQ(v, i);
    ++i;
  }
}

TEST(RingBufferTest, NonTrivialString) {
  RingBuffer<String> rb;
  rb.push_back(String("a"));
  rb.push_back(String("b"));
  rb.emplace_back("c");

  EXPECT_EQ(rb.size(), 3);
  EXPECT_EQ(rb.front(), String("a"));
  EXPECT_EQ(rb.back(), String("c"));

  rb.pop_front();
  EXPECT_EQ(rb.front(), String("b"));

  // grow with non-trivial elements
  for (int i = 0; i < 100; ++i) {
    rb.push_back(String("x"));
  }
  EXPECT_EQ(rb.size(), 102);
  EXPECT_EQ(rb[0], String("b"));
  EXPECT_EQ(rb.back(), String("x"));
}

// ============================================================================
// insert / erase / clear / move
// ============================================================================

TEST(RingBufferTest, InsertErase) {
  RingBuffer<u64> rb;
  rb.push_back(10);
  rb.push_back(30);
  rb.insert(1, 20);

  EXPECT_EQ(rb.size(), 3);
  EXPECT_EQ(rb[0], 10);
  EXPECT_EQ(rb[1], 20);
  EXPECT_EQ(rb[2], 30);

  rb.erase(1);
  EXPECT_EQ(rb.size(), 2);
  EXPECT_EQ(rb[0], 10);
  EXPECT_EQ(rb[1], 30);
}

TEST(RingBufferTest, ClearReuse) {
  RingBuffer<u64> rb;
  for (u64 i = 0; i < 10; ++i) {
    rb.push_back(i);
  }
  rb.clear();
  EXPECT_TRUE(rb.empty());

  rb.push_back(42);
  EXPECT_EQ(rb.size(), 1);
  EXPECT_EQ(rb.front(), 42);
  EXPECT_EQ(rb.back(), 42);
}

TEST(RingBufferTest, MoveConstructor) {
  RingBuffer<u64> src;
  src.push_back(1);
  src.push_back(2);

  RingBuffer<u64> dst(std::move(src));
  EXPECT_EQ(dst.size(), 2);
  EXPECT_EQ(dst.front(), 1);
  EXPECT_EQ(dst.back(), 2);
  EXPECT_EQ(src.size(), 0);
  EXPECT_TRUE(src.empty());
}
