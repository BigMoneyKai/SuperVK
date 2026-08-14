#include <gtest/gtest.h>

#include "core/container/array/array.hpp"
#include "core/container/string/string.hpp"

// ============================================================================
// Construction
// ============================================================================

TEST(ArrayTest, DefaultConstructor) {
  Array<u64> arr;
  EXPECT_EQ(arr.size(), 0);
  EXPECT_EQ(arr.capacity(), 0);
  EXPECT_TRUE(arr.empty());
  EXPECT_NE(arr.allocator(), nullptr);
}

TEST(ArrayTest, CapacityValueConstructor) {
  Array<u64> arr(8, 42);
  EXPECT_EQ(arr.size(), 8);
  EXPECT_GE(arr.capacity(), 8);
  for (u64 i = 0; i < 8; ++i) {
    EXPECT_EQ(arr[i], 42);
  }
}

TEST(ArrayTest, CapacityDefaultValueConstructor) {
  Array<u64> arr(4);
  EXPECT_EQ(arr.size(), 4);
  for (u64 i = 0; i < 4; ++i) {
    EXPECT_EQ(arr[i], 0);
  }
}

TEST(ArrayTest, DataSizeConstructor) {
  // NOTE: Array(void* data, u64 size) copies 'size' BYTES (not elements).
  // Use u8 so byte count == element count.
  u8 raw[] = {10, 20, 30, 40, 50};
  Array<u8> arr(raw, 5);
  EXPECT_EQ(arr.size(), 5);
  for (u64 i = 0; i < 5; ++i) {
    EXPECT_EQ(arr[i], raw[i]);
  }
}

// ============================================================================
// Move semantics
// ============================================================================

TEST(ArrayTest, MoveConstructor) {
  Array<u64> src(3, 99);
  u64 *srcData = src.data();
  u64 srcSize = src.size();

  Array<u64> dst(std::move(src));

  EXPECT_EQ(dst.size(), srcSize);
  EXPECT_EQ(dst.data(), srcData);
  EXPECT_EQ(src.data(), nullptr);
  EXPECT_EQ(src.size(), 0);
  EXPECT_EQ(src.capacity(), 0);
}

TEST(ArrayTest, MoveAssignment) {
  Array<u64> src(3, 77);
  u64 *srcData = src.data();

  Array<u64> dst;
  dst = std::move(src);

  EXPECT_EQ(dst.size(), 3);
  EXPECT_EQ(dst.data(), srcData);
  for (u64 i = 0; i < 3; ++i) {
    EXPECT_EQ(dst[i], 77);
  }
  EXPECT_EQ(src.data(), nullptr);
}

// ============================================================================
// push_back
// ============================================================================

TEST(ArrayTest, PushBackLvalue) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(20);
  arr.push_back(30);

  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
  EXPECT_EQ(arr[2], 30);
}

TEST(ArrayTest, PushBackRvalue) {
  Array<String> arr;
  String s("hello");
  arr.push_back(std::move(s));

  EXPECT_EQ(arr.size(), 1);
  EXPECT_EQ(arr[0], String("hello"));
}

TEST(ArrayTest, PushBackLargeScale) {
  Array<u64> arr;
  constexpr u64 N = 10000;
  for (u64 i = 0; i < N; ++i) {
    arr.push_back(i);
  }
  EXPECT_EQ(arr.size(), N);
  for (u64 i = 0; i < N; ++i) {
    EXPECT_EQ(arr[i], i);
  }
}

TEST(ArrayTest, PushBackTriviallyCopyable) {
  // Verify push_back works with trivial types (memcpy path)
  Array<f32> arr;
  for (u64 i = 0; i < 512; ++i) {
    arr.push_back(static_cast<f32>(i) * 1.5f);
  }
  EXPECT_EQ(arr.size(), 512);
  for (u64 i = 0; i < 512; ++i) {
    EXPECT_FLOAT_EQ(arr[i], static_cast<f32>(i) * 1.5f);
  }
}

// ============================================================================
// emplace_back
// ============================================================================

TEST(ArrayTest, EmplaceBack) {
  Array<String> arr;
  arr.emplace_back("hello world");
  arr.emplace_back(5, 'x');

  EXPECT_EQ(arr.size(), 2);
  EXPECT_EQ(arr[0], String("hello world"));
  EXPECT_EQ(arr[1], String("xxxxx"));
}

TEST(ArrayTest, EmplaceBackTrivial) {
  Array<u64> arr;
  arr.emplace_back(42);
  arr.emplace_back(99);

  EXPECT_EQ(arr.size(), 2);
  EXPECT_EQ(arr[0], 42);
  EXPECT_EQ(arr[1], 99);
}

// ============================================================================
// pop_back
// ============================================================================

TEST(ArrayTest, PopBack) {
  Array<u64> arr;
  arr.push_back(1);
  arr.push_back(2);
  arr.push_back(3);

  arr.pop_back();
  EXPECT_EQ(arr.size(), 2);
  EXPECT_EQ(arr.back(), 2);

  arr.pop_back();
  EXPECT_EQ(arr.size(), 1);
  EXPECT_EQ(arr.back(), 1);

  arr.pop_back();
  EXPECT_EQ(arr.size(), 0);
  EXPECT_TRUE(arr.empty());
}

// ============================================================================
// insert
// ============================================================================

TEST(ArrayTest, InsertLvalueAtFront) {
  Array<u64> arr;
  arr.push_back(20);
  arr.push_back(30);

  arr.insert(0, 10);
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
  EXPECT_EQ(arr[2], 30);
}

TEST(ArrayTest, InsertLvalueAtMiddle) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(30);

  arr.insert(1, 20);
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
  EXPECT_EQ(arr[2], 30);
}

TEST(ArrayTest, InsertLvalueAtBack) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(20);

  arr.insert(2, 30);
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[2], 30);
}

TEST(ArrayTest, InsertRvalue) {
  Array<String> arr;
  arr.push_back(String("first"));
  arr.push_back(String("third"));

  arr.insert(1, String("second"));
  EXPECT_EQ(arr[0], String("first"));
  EXPECT_EQ(arr[1], String("second"));
  EXPECT_EQ(arr[2], String("third"));
}

TEST(ArrayTest, InsertIntoEmpty) {
  Array<u64> arr;
  arr.insert(0, 42);
  EXPECT_EQ(arr.size(), 1);
  EXPECT_EQ(arr[0], 42);
}

// ============================================================================
// erase
// ============================================================================

TEST(ArrayTest, EraseFromFront) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(20);
  arr.push_back(30);

  arr.erase(0);
  EXPECT_EQ(arr.size(), 2);
  EXPECT_EQ(arr[0], 20);
  EXPECT_EQ(arr[1], 30);
}

TEST(ArrayTest, EraseFromMiddle) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(20);
  arr.push_back(30);

  arr.erase(1);
  EXPECT_EQ(arr.size(), 2);
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 30);
}

TEST(ArrayTest, EraseFromBack) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(20);
  arr.push_back(30);

  arr.erase(2);
  EXPECT_EQ(arr.size(), 2);
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
}

// ============================================================================
// clear
// ============================================================================

TEST(ArrayTest, Clear) {
  Array<u64> arr;
  for (u64 i = 0; i < 100; ++i) {
    arr.push_back(i);
  }
  EXPECT_FALSE(arr.empty());

  arr.clear();
  EXPECT_EQ(arr.size(), 0);
  EXPECT_TRUE(arr.empty());
  // Capacity should be preserved after clear
  EXPECT_GE(arr.capacity(), 100);
}

// ============================================================================
// resize
// ============================================================================

TEST(ArrayTest, ResizeGrow) {
  Array<u64> arr;
  arr.push_back(1);
  arr.push_back(2);

  arr.resize(5, 99);
  EXPECT_EQ(arr.size(), 5);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 99);
  EXPECT_EQ(arr[3], 99);
  EXPECT_EQ(arr[4], 99);
}

TEST(ArrayTest, ResizeShrink) {
  Array<u64> arr;
  arr.push_back(1);
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);

  arr.resize(2);
  EXPECT_EQ(arr.size(), 2);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
}

// ============================================================================
// reserve
// ============================================================================

TEST(ArrayTest, Reserve) {
  Array<u64> arr;
  arr.reserve(1024);
  EXPECT_GE(arr.capacity(), 1024);
  EXPECT_EQ(arr.size(), 0); // reserve does not change size
}

TEST(ArrayTest, ReserveSmallerDoesNotShrink) {
  Array<u64> arr;
  arr.reserve(512);
  u64 cap = arr.capacity();
  arr.reserve(128); // should be a no-op when requesting less
  EXPECT_GE(arr.capacity(), cap);
}

// ============================================================================
// Element access
// ============================================================================

TEST(ArrayTest, OperatorBracket) {
  Array<u64> arr(3, 0);
  arr[0] = 100;
  arr[1] = 200;
  arr[2] = 300;

  EXPECT_EQ(arr[0], 100);
  EXPECT_EQ(arr[1], 200);
  EXPECT_EQ(arr[2], 300);
}

TEST(ArrayTest, FrontBack) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(20);
  arr.push_back(30);

  EXPECT_EQ(arr.front(), 10);
  EXPECT_EQ(arr.back(), 30);

  arr.front() = 99;
  arr.back() = 77;
  EXPECT_EQ(arr[0], 99);
  EXPECT_EQ(arr[2], 77);
}

TEST(ArrayTest, FrontBackConst) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(20);

  const auto &carr = arr;
  EXPECT_EQ(carr.front(), 10);
  EXPECT_EQ(carr.back(), 20);
}

TEST(ArrayTest, Data) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(20);

  u64 *d = arr.data();
  EXPECT_EQ(d[0], 10);
  EXPECT_EQ(d[1], 20);

  const auto &carr = arr;
  const u64 *cd = carr.data();
  EXPECT_EQ(cd[0], 10);
  EXPECT_EQ(cd[1], 20);
}

// ============================================================================
// append
// ============================================================================

TEST(ArrayTest, Append) {
  Array<u64> arr;
  arr.push_back(1);
  arr.push_back(2);

  u64 extra[] = {3, 4, 5};
  arr.append(extra, 3);

  EXPECT_EQ(arr.size(), 5);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);
  EXPECT_EQ(arr[3], 4);
  EXPECT_EQ(arr[4], 5);
}

TEST(ArrayTest, AppendEmpty) {
  Array<u64> arr;
  u64 extra[] = {1, 2, 3};
  arr.append(extra, 3);
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[2], 3);
}

// ============================================================================
// Iterators
// ============================================================================

TEST(ArrayTest, ForwardIteration) {
  Array<u64> arr;
  for (u64 i = 0; i < 10; ++i) {
    arr.push_back(i * 10);
  }

  u64 expected = 0;
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    EXPECT_EQ(*it, expected * 10);
    ++expected;
  }
  EXPECT_EQ(expected, 10);
}

TEST(ArrayTest, ConstIteration) {
  Array<u64> arr;
  for (u64 i = 0; i < 5; ++i) {
    arr.push_back(i);
  }

  const auto &carr = arr;
  u64 sum = 0;
  for (auto it = carr.begin(); it != carr.end(); ++it) {
    sum += *it;
  }
  EXPECT_EQ(sum, 10); // 0+1+2+3+4
}

TEST(ArrayTest, RangeBasedFor) {
  Array<u64> arr;
  for (u64 i = 0; i < 5; ++i) {
    arr.push_back(i);
  }

  u64 sum = 0;
  for (auto &v : arr) {
    sum += v;
  }
  EXPECT_EQ(sum, 10);
}

TEST(ArrayTest, ReverseIteration) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(20);
  arr.push_back(30);

  auto rit = arr.rbegin();
  EXPECT_EQ(*rit, 30);
  ++rit;
  EXPECT_EQ(*rit, 20);
  ++rit;
  EXPECT_EQ(*rit, 10);
  ++rit;
  EXPECT_EQ(rit, arr.rend());
}

TEST(ArrayTest, ConstReverseIteration) {
  Array<u64> arr;
  arr.push_back(1);
  arr.push_back(2);
  arr.push_back(3);

  const auto &carr = arr;
  u64 sum = 0;
  for (auto it = carr.rbegin(); it != carr.rend(); ++it) {
    sum += *it;
  }
  EXPECT_EQ(sum, 6);
}

// ============================================================================
// empty / size / capacity
// ============================================================================

TEST(ArrayTest, Empty) {
  Array<u64> arr;
  EXPECT_TRUE(arr.empty());

  arr.push_back(1);
  EXPECT_FALSE(arr.empty());

  arr.clear();
  EXPECT_TRUE(arr.empty());
}

TEST(ArrayTest, SizeAndCapacity) {
  Array<u64> arr;
  EXPECT_EQ(arr.size(), 0);
  EXPECT_EQ(arr.capacity(), 0);

  arr.reserve(100);
  EXPECT_EQ(arr.size(), 0);
  EXPECT_GE(arr.capacity(), 100);

  arr.push_back(1);
  EXPECT_EQ(arr.size(), 1);
}

// ============================================================================
// allocator
// ============================================================================

TEST(ArrayTest, Allocator) {
  Array<u64> arr;
  EXPECT_NE(arr.allocator(), nullptr);

  const auto &carr = arr;
  EXPECT_NE(carr.allocator(), nullptr);
}

// ============================================================================
// Non-trivially-copyable type
// ============================================================================

TEST(ArrayTest, NonTrivialType) {
  // String is non-trivially-copyable — exercises placement-new path
  Array<String> arr;
  arr.push_back(String("hello"));
  arr.emplace_back("world");
  arr.insert(1, String("middle"));

  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], String("hello"));
  EXPECT_EQ(arr[1], String("middle"));
  EXPECT_EQ(arr[2], String("world"));
}

TEST(ArrayTest, NonTrivialErase) {
  Array<String> arr;
  arr.push_back(String("a"));
  arr.push_back(String("b"));
  arr.push_back(String("c"));

  arr.erase(1);
  EXPECT_EQ(arr.size(), 2);
  EXPECT_EQ(arr[0], String("a"));
  EXPECT_EQ(arr[1], String("c"));
}

// ============================================================================
// Regression: insert const T& (lvalue) overload
// ============================================================================

TEST(ArrayTest, InsertRealLvalueAtBack) {
  Array<u64> arr;
  arr.push_back(10);
  arr.push_back(20);

  u64 v = 30; // real lvalue -> const T& overload
  arr.insert(
      2, v); // legal append at the back; aborts when the assert was inverted
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
  EXPECT_EQ(arr[2], 30);
}

// ============================================================================
// Regression: resize growth within capacity
// ============================================================================

TEST(ArrayTest, ResizeGrowWithinCapacityKeepsCapacity) {
  Array<u64> arr;
  arr.reserve(8);
  arr.push_back(1);
  arr.push_back(2);
  const u64 capBefore = arr.capacity();

  arr.resize(5, 99);
  EXPECT_EQ(arr.size(), 5);
  EXPECT_EQ(arr.capacity(), capBefore);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 99);
  EXPECT_EQ(arr[3], 99);
  EXPECT_EQ(arr[4], 99);
}

TEST(ArrayTest, ResizeGrowWithinCapacityNonTrivial) {
  // Regression: used to call the destructor on unconstructed slots; String
  // would SEGV
  Array<String> arr;
  arr.reserve(8);
  arr.push_back(String("a"));
  arr.push_back(String("b"));

  arr.resize(5, String("x"));
  EXPECT_EQ(arr.size(), 5);
  EXPECT_EQ(arr[0], String("a"));
  EXPECT_EQ(arr[1], String("b"));
  EXPECT_EQ(arr[2], String("x"));
  EXPECT_EQ(arr[3], String("x"));
  EXPECT_EQ(arr[4], String("x"));
}

// ============================================================================
// Regression: after resize growth, capacity must match the allocation so
// push_back can keep working
// ============================================================================

TEST(ArrayTest, ResizeThenPushBack) {
  Array<u64> arr;
  arr.push_back(1);
  arr.push_back(2);

  arr.resize(5, 99);  // old bug: allocated 5 but claimed capacity 7
  arr.push_back(100); // old bug: out-of-bounds write here
  arr.push_back(101);
  arr.push_back(102); // 8th element, triggers another growth

  EXPECT_EQ(arr.size(), 8);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 99);
  EXPECT_EQ(arr[5], 100);
  EXPECT_EQ(arr[6], 101);
  EXPECT_EQ(arr[7], 102);
}

// ============================================================================
// Regression: resize growth must destroy old elements (non-trivial types)
// ============================================================================

namespace {

struct CountingType {
  static int alive;
  int value;

  explicit CountingType(int v) : value(v) { ++alive; }
  CountingType(const CountingType &o) : value(o.value) { ++alive; }
  CountingType(CountingType &&o) noexcept : value(o.value) { ++alive; }
  CountingType &operator=(const CountingType &) = default;
  CountingType &operator=(CountingType &&) = default;
  ~CountingType() { --alive; }
};

int CountingType::alive = 0;

} // namespace

TEST(ArrayTest, ResizeGrowDestroysOldElements) {
  CountingType::alive = 0;
  {
    Array<CountingType> arr;
    arr.push_back(CountingType(1));
    arr.push_back(CountingType(2));

    // growth beyond capacity: old elements must be destroyed after being
    // moved, otherwise they leak
    arr.resize(5, CountingType(99));
    EXPECT_EQ(arr.size(), 5);
    EXPECT_EQ(CountingType::alive, 5);

    EXPECT_EQ(arr[0].value, 1);
    EXPECT_EQ(arr[1].value, 2);
    EXPECT_EQ(arr[2].value, 99);
    EXPECT_EQ(arr[3].value, 99);
    EXPECT_EQ(arr[4].value, 99);
  }
  EXPECT_EQ(CountingType::alive, 0);
}
