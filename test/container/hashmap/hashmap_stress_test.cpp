#include <gtest/gtest.h>

#include "core/container/hashmap/hashmap.hpp"
#include "core/container/string/string.hpp"

#include <random>
#include <string>

// ============================================================================
// Large-scale insert / find / erase / clear
// ============================================================================

TEST(HashMapStressTest, InsertFindEraseTwoHundredK) {
  HashMap<u64, u64> map(64);
  constexpr u64 N = 200'000;

  for (u64 i = 0; i < N; ++i) {
    map.insert(i, i * 3 + 1);
  }
  ASSERT_EQ(map.size(), N);

  for (u64 i = 0; i < N; ++i) {
    auto it = map.find(i);
    ASSERT_NE(it, map.end());
    EXPECT_EQ(it->val, i * 3 + 1);
  }

  // erase even keys
  for (u64 i = 0; i < N; i += 2) {
    map.erase(i);
  }
  EXPECT_EQ(map.size(), N / 2);

  // erased keys must be gone, the rest must still be found
  for (u64 i = 0; i < N; ++i) {
    const bool present = (i % 2) == 1;
    EXPECT_EQ(map.contains(i), present);
    if (present) {
      auto it = map.find(i);
      ASSERT_NE(it, map.end());
      EXPECT_EQ(it->val, i * 3 + 1);
    }
  }

  // erase the rest
  for (u64 i = 1; i < N; i += 2) {
    map.erase(i);
  }
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
}

// ============================================================================
// Random insert / erase churn (fixed seed; size bookkeeping must stay
// consistent)
// ============================================================================

TEST(HashMapStressTest, RandomInsertEraseChurn) {
  HashMap<u64, u64> map(64);
  std::mt19937 rng(12345);
  constexpr u64 N = 100'000;
  u64 inserted = 0;

  for (u64 step = 0; step < N; ++step) {
    const u64 key = static_cast<u64>(rng()) % 20'000;
    if (rng() % 2 == 0) {
      if (!map.contains(key)) {
        ++inserted;
      }
      map.insert(key, step);
    } else {
      if (map.contains(key)) {
        --inserted;
      }
      map.erase(key);
    }
  }

  EXPECT_EQ(map.size(), inserted);
  map.clear();
  EXPECT_TRUE(map.empty());
}

// ============================================================================
// Large-scale String keys
// ============================================================================

TEST(HashMapStressTest, StringKeysHeavy) {
  HashMap<String, u64> map(64);
  constexpr u64 N = 20'000;

  for (u64 i = 0; i < N; ++i) {
    std::string s = std::to_string(i);
    map.insert(String(s.c_str()), i);
  }
  ASSERT_EQ(map.size(), N);

  for (u64 i = 0; i < N; ++i) {
    std::string s = std::to_string(i);
    auto it = map.find(String(s.c_str()));
    ASSERT_NE(it, map.end());
    EXPECT_EQ(it->val, i);
  }

  for (u64 i = 0; i < N; i += 2) {
    std::string s = std::to_string(i);
    map.erase(String(s.c_str()));
  }
  EXPECT_EQ(map.size(), N / 2);
  EXPECT_FALSE(map.contains(String("0")));
  EXPECT_TRUE(map.contains(String("1")));

  map.clear();
  EXPECT_TRUE(map.empty());
}
