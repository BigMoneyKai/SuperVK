#include <gtest/gtest.h>

#include "core/container/hashmap/hashmap.hpp"
#include "core/container/string/string.hpp"

// NOTE: HashMap has known const-correctness limitations:
// - const_iterator cannot be constructed from const data pointers
// - operator[] const calls non-const end()
// - emplace is broken (calls insert with 1 arg instead of 2)
// These tests exercise only the working API surface.

// ============================================================================
// Construction
// ============================================================================

TEST(HashMapTest, Constructor) {
  HashMap<u64, u64> map(16);
  EXPECT_EQ(map.size(), 0);
  EXPECT_GE(map.capacity(), 16);
  EXPECT_TRUE(map.empty());
}

TEST(HashMapTest, ConstructorLargeCapacity) {
  HashMap<u64, u64> map(1024);
  EXPECT_GE(map.capacity(), 1024);
  EXPECT_TRUE(map.empty());
}

// ============================================================================
// Move semantics
// ============================================================================

TEST(HashMapTest, MoveConstructor) {
  HashMap<u64, String> map(16);
  map.insert(1, String("one"));
  map.insert(2, String("two"));
  u64 movedSize = map.size();

  HashMap<u64, String> dst(std::move(map));

  EXPECT_EQ(dst.size(), movedSize);
  EXPECT_EQ(dst[1], String("one"));
  EXPECT_EQ(dst[2], String("two"));
}

TEST(HashMapTest, MoveAssignment) {
  HashMap<u64, u64> src(8);
  src.insert(10, 100);
  src.insert(20, 200);

  HashMap<u64, u64> dst(8);
  dst = std::move(src);

  EXPECT_EQ(dst.size(), 2);
  EXPECT_EQ(dst[10], 100);
  EXPECT_EQ(dst[20], 200);
}

// ============================================================================
// insert — all 4 overloads
// ============================================================================

TEST(HashMapTest, InsertLvalueLvalue) {
  HashMap<u64, String> map(8);
  const u64 key = 1;
  const String val("value");
  map.insert(key, val);

  EXPECT_EQ(map.size(), 1);
  EXPECT_TRUE(map.contains(1));
  EXPECT_EQ(map[1], String("value"));
}

TEST(HashMapTest, InsertLvalueRvalue) {
  HashMap<u64, String> map(8);
  const u64 key = 1;
  map.insert(key, String("moved"));

  EXPECT_EQ(map.size(), 1);
  EXPECT_EQ(map[1], String("moved"));
}

TEST(HashMapTest, InsertRvalueLvalue) {
  HashMap<String, u64> map(8);
  const u64 val = 42;
  map.insert(String("key"), val);

  EXPECT_EQ(map.size(), 1);
  EXPECT_EQ(map[String("key")], 42);
}

TEST(HashMapTest, InsertRvalueRvalue) {
  HashMap<String, String> map(8);
  map.insert(String("k"), String("v"));

  EXPECT_EQ(map.size(), 1);
  EXPECT_EQ(map[String("k")], String("v"));
}

TEST(HashMapTest, InsertDuplicate) {
  HashMap<u64, u64> map(8);
  map.insert(1, 10);
  map.insert(1, 99); // overwrite

  EXPECT_EQ(map.size(), 1);
  EXPECT_EQ(map[1], 99);
}

TEST(HashMapTest, InsertMany) {
  HashMap<u64, u64> map(16);
  constexpr u64 N = 500;
  for (u64 i = 0; i < N; ++i) {
    map.insert(i, i * 10);
  }
  EXPECT_EQ(map.size(), N);
  for (u64 i = 0; i < N; ++i) {
    EXPECT_EQ(map[i], i * 10);
  }
}

// ============================================================================
// find
// ============================================================================

TEST(HashMapTest, FindExisting) {
  HashMap<u64, String> map(8);
  map.insert(1, String("one"));
  map.insert(2, String("two"));
  map.insert(3, String("three"));

  auto it = map.find(2);
  EXPECT_NE(it, map.end());
  EXPECT_EQ(it.second(), String("two"));
  EXPECT_EQ(it.first(), 2);
}

TEST(HashMapTest, FindNonExisting) {
  HashMap<u64, String> map(8);
  map.insert(1, String("one"));

  auto it = map.find(99);
  EXPECT_EQ(it, map.end());
}

// ============================================================================
// operator[]
// ============================================================================

TEST(HashMapTest, OperatorBracketRead) {
  HashMap<u64, u64> map(8);
  map.insert(10, 100);
  map.insert(20, 200);

  EXPECT_EQ(map[10], 100);
  EXPECT_EQ(map[20], 200);
}

// ============================================================================
// contains
// ============================================================================

TEST(HashMapTest, Contains) {
  HashMap<u64, u64> map(8);
  map.insert(42, 99);

  EXPECT_TRUE(map.contains(42));
  EXPECT_FALSE(map.contains(0));
  EXPECT_FALSE(map.contains(100));
}

// ============================================================================
// erase
// ============================================================================

TEST(HashMapTest, EraseExisting) {
  HashMap<u64, String> map(8);
  map.insert(1, String("one"));
  map.insert(2, String("two"));
  map.insert(3, String("three"));

  map.erase(2);
  EXPECT_EQ(map.size(), 2);
  EXPECT_FALSE(map.contains(2));
  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(3));
}

TEST(HashMapTest, EraseNonExisting) {
  HashMap<u64, u64> map(8);
  map.insert(1, 10);

  map.erase(99); // should not crash
  EXPECT_EQ(map.size(), 1);
}

TEST(HashMapTest, EraseAll) {
  HashMap<u64, u64> map(16);
  for (u64 i = 0; i < 100; ++i) {
    map.insert(i, i);
  }
  for (u64 i = 0; i < 100; ++i) {
    map.erase(i);
  }
  EXPECT_EQ(map.size(), 0);
  EXPECT_TRUE(map.empty());
}

// ============================================================================
// clear
// ============================================================================

TEST(HashMapTest, Clear) {
  HashMap<u64, u64> map(16);
  for (u64 i = 0; i < 50; ++i) {
    map.insert(i, i);
  }
  EXPECT_FALSE(map.empty());

  map.clear();
  EXPECT_EQ(map.size(), 0);
  EXPECT_TRUE(map.empty());
}

// ============================================================================
// rehash
// ============================================================================

TEST(HashMapTest, RehashLarger) {
  HashMap<u64, u64> map(16);
  for (u64 i = 0; i < 100; ++i) {
    map.insert(i, i);
  }

  u64 oldCap = map.capacity();
  map.rehash(512);
  EXPECT_GE(map.capacity(), 512);

  // All entries should still be accessible
  for (u64 i = 0; i < 100; ++i) {
    EXPECT_EQ(map[i], i);
  }
}

// ============================================================================
// Iterators (non-const only — const iterators are broken)
// ============================================================================

TEST(HashMapTest, ForwardIteration) {
  HashMap<u64, u64> map(16);
  map.insert(1, 10);
  map.insert(2, 20);
  map.insert(3, 30);

  u64 sum = 0;
  u64 count = 0;
  for (auto it = map.begin(); it != map.end(); ++it) {
    sum += it.second();
    ++count;
  }
  EXPECT_EQ(count, 3);
  EXPECT_EQ(sum, 60);
}

TEST(HashMapTest, RangeBasedFor) {
  HashMap<u64, u64> map(16);
  for (u64 i = 0; i < 10; ++i) {
    map.insert(i, i * 10);
  }

  u64 count = 0;
  for (const auto &entry : map) {
    EXPECT_EQ(entry.val, entry.key * 10);
    ++count;
  }
  EXPECT_EQ(count, 10);
}

TEST(HashMapTest, EmptyIteration) {
  HashMap<u64, u64> map(16);
  EXPECT_EQ(map.begin(), map.end());
}

// ============================================================================
// size / capacity / empty
// ============================================================================

TEST(HashMapTest, SizeCapacityEmpty) {
  HashMap<u64, u64> map(32);
  EXPECT_EQ(map.size(), 0);
  EXPECT_GE(map.capacity(), 32);
  EXPECT_TRUE(map.empty());

  map.insert(1, 10);
  EXPECT_EQ(map.size(), 1);
  EXPECT_FALSE(map.empty());
}

// ============================================================================
// String keys
// ============================================================================

TEST(HashMapTest, StringKeys) {
  HashMap<String, u64> map(16);
  map.insert(String("apple"), 1);
  map.insert(String("banana"), 2);
  map.insert(String("cherry"), 3);

  EXPECT_EQ(map.size(), 3);
  EXPECT_TRUE(map.contains(String("apple")));
  EXPECT_EQ(map[String("banana")], 2);
  EXPECT_EQ(map[String("cherry")], 3);
}
