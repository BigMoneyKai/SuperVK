#include <gtest/gtest.h>

#include "core/container/array/array.hpp"
#include "core/container/string/string.hpp"

// ============================================================================
// Large-scale push / pop / clear
// ============================================================================

TEST(ArrayStressTest, PushBackMillionU64) {
  Array<u64> arr;
  constexpr u64 N = 1'000'000;

  for (u64 i = 0; i < N; ++i) {
    arr.push_back(i);
  }
  ASSERT_EQ(arr.size(), N);
  EXPECT_GE(arr.capacity(), N);
  EXPECT_EQ(arr[0], 0);
  EXPECT_EQ(arr[N - 1], N - 1);
  for (u64 i = 0; i < N; i += 100'000) {
    EXPECT_EQ(arr[i], i);
  }

  // Delete half
  for (u64 i = 0; i < N / 2; ++i) {
    arr.pop_back();
  }
  EXPECT_EQ(arr.size(), N / 2);
  EXPECT_EQ(arr.back(), N / 2 - 1);
}

TEST(ArrayStressTest, PushBackStringSSO) {
  Array<String> arr;
  const char *s = "Rick\'N\'Morty"; // <= SSO
  constexpr u64 N = 50'000;

  for (u64 i = 0; i < N; ++i) {
    arr.push_back(String(s));
  }
  ASSERT_EQ(arr.size(), N);
  EXPECT_EQ(arr.front(), String(s));
  EXPECT_EQ(arr.back(), String(s));

  arr.clear();
  EXPECT_TRUE(arr.empty());
  EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayStressTest, PushBackStringHeap) {
  Array<String> arr;
  const char *s = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFKaiLovingNaomiFore"
                  "verForeverForeverForever"; // > SSO
  constexpr u64 N = 50'000;

  for (u64 i = 0; i < N; ++i) {
    arr.push_back(String(s));
  }
  ASSERT_EQ(arr.size(), N);
  EXPECT_EQ(arr.front(), String(s));
  EXPECT_EQ(arr.back(), String(s));

  arr.clear();
  EXPECT_TRUE(arr.empty());
  EXPECT_EQ(arr.size(), 0);
}

// ============================================================================
// Big scale of "resize"
// ============================================================================

TEST(ArrayStressTest, ResizeAcrossBoundaries) {
  Array<u64> arr;

  arr.resize(10'000, 1);
  EXPECT_EQ(arr.size(), 10'000);

  arr.resize(200'000, 2); // expand
  EXPECT_EQ(arr.size(), 200'000);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[9'999], 1);
  EXPECT_EQ(arr[10'000], 2);

  arr.resize(5'000, 3); // shrink
  EXPECT_EQ(arr.size(), 5'000);
  EXPECT_EQ(arr[4'999], 1);

  arr.resize(50'000, 4); // increase within capacity
  EXPECT_EQ(arr.size(), 50'000);
  EXPECT_EQ(arr[4'999], 1);
  EXPECT_EQ(arr[5'000], 4);

  for (u64 i = 0; i < 25'000; ++i) {
    arr.pop_back();
  }
  EXPECT_EQ(arr.size(), 25'000);
}

// ============================================================================
// Large-scale insert / erase / pop churn
// ============================================================================

TEST(ArrayStressTest, InsertEraseChurn) {
  Array<u64> arr;
  constexpr u64 N = 5'000'000;
  for (u64 i = 0; i < N; ++i) {
    arr.push_back(i);
  }

  // 500 inserts at the middle (lvalue overload)
  for (u64 k = 0; k < 500; ++k) {
    const u64 v = 999'999;
    arr.insert(arr.size() / 2, v);
  }
  EXPECT_EQ(arr.size(), N + 500);
  EXPECT_EQ(arr[0], 0);
  EXPECT_EQ(arr.back(), N - 1);

  // 200 inserts at the front
  for (u64 k = 0; k < 200; ++k) {
    arr.insert(0, k);
  }
  EXPECT_EQ(arr.size(), N + 700);
  EXPECT_EQ(arr[0], 199);

  // 500 erases at the middle
  for (u64 k = 0; k < 500; ++k) {
    arr.erase(arr.size() / 2);
  }
  EXPECT_EQ(arr.size(), N + 200);
  EXPECT_EQ(arr[0], 199);

  // 200 erases at the back
  for (u64 k = 0; k < 200; ++k) {
    arr.pop_back();
  }
  EXPECT_EQ(arr.size(), N);

  arr.clear();
  EXPECT_TRUE(arr.empty());
}
