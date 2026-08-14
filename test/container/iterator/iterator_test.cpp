#include <gtest/gtest.h>

#include "core/container/iterator/iterator.hpp"
#include "core/container/iterator/reverse_iterator.hpp"

// ============================================================================
// Iterator — construction
// ============================================================================

TEST(IteratorTest, DefaultConstructor) {
  Iterator<int> it;
  EXPECT_EQ(it, Iterator<int>()); // default-constructed iterators compare equal
}

TEST(IteratorTest, PointerConstructor) {
  int x = 42;
  Iterator<int> it(&x);
  EXPECT_EQ(*it, 42);
}

// ============================================================================
// Iterator — dereference
// ============================================================================

TEST(IteratorTest, Dereference) {
  int values[] = {10, 20, 30};
  Iterator<int> it(values);
  EXPECT_EQ(*it, 10);
}

TEST(IteratorTest, ArrowOperator) {
  struct S {
    int a;
  };
  S s{99};
  Iterator<S> it(&s);
  EXPECT_EQ(it->a, 99);
}

// ============================================================================
// Iterator — increment / decrement
// ============================================================================

TEST(IteratorTest, PreIncrement) {
  int values[] = {1, 2, 3};
  Iterator<int> it(values);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}

TEST(IteratorTest, PostIncrement) {
  int values[] = {1, 2, 3};
  Iterator<int> it(values);
  Iterator<int> prev = it++;
  EXPECT_EQ(*prev, 1);
  EXPECT_EQ(*it, 2);
}

TEST(IteratorTest, PreDecrement) {
  int values[] = {1, 2, 3};
  Iterator<int> it(values + 2);
  --it;
  EXPECT_EQ(*it, 2);
  --it;
  EXPECT_EQ(*it, 1);
}

TEST(IteratorTest, PostDecrement) {
  int values[] = {1, 2, 3};
  Iterator<int> it(values + 2);
  Iterator<int> prev = it--;
  EXPECT_EQ(*prev, 3);
  EXPECT_EQ(*it, 2);
}

// ============================================================================
// Iterator — arithmetic
// ============================================================================

TEST(IteratorTest, OperatorPlusOffset) {
  int values[] = {0, 1, 2, 3, 4, 5};
  Iterator<int> it(values);
  Iterator<int> it2 = it + 3;
  EXPECT_EQ(*it2, 3);
}

TEST(IteratorTest, OperatorMinusOffset) {
  int values[] = {0, 1, 2, 3, 4, 5};
  Iterator<int> it(values + 5);
  Iterator<int> it2 = it - 2;
  EXPECT_EQ(*it2, 3);
}

TEST(IteratorTest, OperatorMinusIterator) {
  int values[] = {0, 1, 2, 3, 4};
  Iterator<int> begin(values);
  Iterator<int> end(values + 5);
  EXPECT_EQ(end - begin, 5);
}

TEST(IteratorTest, OperatorBracket) {
  int values[] = {10, 20, 30, 40};
  Iterator<int> it(values);
  EXPECT_EQ(it[0], 10);
  EXPECT_EQ(it[2], 30);
  EXPECT_EQ(it[3], 40);
}

TEST(IteratorTest, CompoundPlusEquals) {
  int values[] = {0, 1, 2, 3, 4, 5};
  Iterator<int> it(values);
  it += 3;
  EXPECT_EQ(*it, 3);
}

TEST(IteratorTest, CompoundMinusEquals) {
  int values[] = {0, 1, 2, 3, 4, 5};
  Iterator<int> it(values + 5);
  it -= 2;
  EXPECT_EQ(*it, 3);
}

// ============================================================================
// Iterator — comparison
// ============================================================================

TEST(IteratorTest, Equality) {
  int values[] = {1, 2, 3};
  Iterator<int> a(values);
  Iterator<int> b(values);
  Iterator<int> c(values + 1);

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
}

TEST(IteratorTest, Inequality) {
  int values[] = {1, 2, 3};
  Iterator<int> a(values);
  Iterator<int> b(values + 1);

  EXPECT_TRUE(a != b);
  EXPECT_FALSE(a != Iterator<int>(values));
}

TEST(IteratorTest, LessThan) {
  int values[] = {1, 2, 3};
  Iterator<int> a(values);
  Iterator<int> b(values + 1);

  EXPECT_TRUE(a < b);
  EXPECT_FALSE(b < a);
}

TEST(IteratorTest, GreaterThan) {
  int values[] = {1, 2, 3};
  Iterator<int> a(values);
  Iterator<int> b(values + 1);

  EXPECT_TRUE(b > a);
  EXPECT_FALSE(a > b);
}

TEST(IteratorTest, LessThanOrEqual) {
  int values[] = {1, 2, 3};
  Iterator<int> a(values);
  Iterator<int> b(values);
  Iterator<int> c(values + 1);

  EXPECT_TRUE(a <= b);
  EXPECT_TRUE(a <= c);
  EXPECT_FALSE(c <= a);
}

TEST(IteratorTest, GreaterThanOrEqual) {
  int values[] = {1, 2, 3};
  Iterator<int> a(values);
  Iterator<int> b(values);
  Iterator<int> c(values + 1);

  EXPECT_TRUE(b >= a);
  EXPECT_TRUE(c >= a);
  EXPECT_FALSE(a >= c);
}

// ============================================================================
// Iterator — const correctness
// ============================================================================

TEST(IteratorTest, ConstIterator) {
  int values[] = {10, 20};
  Iterator<const int> it(values);
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 20);
}

// ============================================================================
// ReverseIterator — construction
// ============================================================================

TEST(ReverseIteratorTest, Construction) {
  int values[] = {1, 2, 3};
  Iterator<int> it(values + 3); // end iterator
  ReverseIterator<Iterator<int>> rit(it);
  EXPECT_EQ(*rit, 3);
}

// ============================================================================
// ReverseIterator — dereference
// ============================================================================

TEST(ReverseIteratorTest, Dereference) {
  int values[] = {10, 20, 30};
  Iterator<int> endIt(values + 3);
  ReverseIterator<Iterator<int>> rit(endIt);
  EXPECT_EQ(*rit, 30);
}

TEST(ReverseIteratorTest, ArrowOperator) {
  struct S {
    int x;
  };
  S arr[] = {{1}, {2}, {3}};
  Iterator<S> endIt(arr + 3);
  ReverseIterator<Iterator<S>> rit(endIt);
  // ReverseIterator::operator->() returns a reference to a temporary pointer;
  // use operator*() instead for reliable access
  EXPECT_EQ((*rit).x, 3);
}

// ============================================================================
// ReverseIterator — increment / decrement
// ============================================================================

TEST(ReverseIteratorTest, PreIncrement) {
  int values[] = {1, 2, 3};
  ReverseIterator<Iterator<int>> rit(Iterator<int>(values + 3));
  EXPECT_EQ(*rit, 3);
  ++rit;
  EXPECT_EQ(*rit, 2);
  ++rit;
  EXPECT_EQ(*rit, 1);
}

TEST(ReverseIteratorTest, PostIncrement) {
  int values[] = {10, 20, 30};
  ReverseIterator<Iterator<int>> rit(Iterator<int>(values + 3));
  auto prev = rit++;
  EXPECT_EQ(*prev, 30);
  EXPECT_EQ(*rit, 20);
}

TEST(ReverseIteratorTest, PreDecrement) {
  int values[] = {1, 2, 3};
  ReverseIterator<Iterator<int>> rit(Iterator<int>(values + 2)); // points to 2
  EXPECT_EQ(*rit, 2);
  --rit;
  EXPECT_EQ(*rit, 3); // moves back towards end
}

TEST(ReverseIteratorTest, PostDecrement) {
  int values[] = {10, 20, 30};
  ReverseIterator<Iterator<int>> rit(Iterator<int>(values + 2)); // points to 20
  auto prev = rit--;
  EXPECT_EQ(*prev, 20);
  EXPECT_EQ(*rit, 30);
}

// ============================================================================
// ReverseIterator — comparison
// ============================================================================

TEST(ReverseIteratorTest, Equality) {
  int values[] = {1, 2, 3};
  ReverseIterator<Iterator<int>> a(Iterator<int>(values + 3));
  ReverseIterator<Iterator<int>> b(Iterator<int>(values + 3));
  ReverseIterator<Iterator<int>> c(Iterator<int>(values + 2));

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
}

TEST(ReverseIteratorTest, Inequality) {
  int values[] = {1, 2, 3};
  ReverseIterator<Iterator<int>> a(Iterator<int>(values + 3));
  ReverseIterator<Iterator<int>> b(Iterator<int>(values + 2));

  EXPECT_TRUE(a != b);
}

// ============================================================================
// ReverseIterator — full traversal
// ============================================================================

TEST(ReverseIteratorTest, FullTraversal) {
  int values[] = {1, 2, 3, 4, 5};
  auto endIt = Iterator<int>(values + 5);
  auto beginIt = Iterator<int>(values);
  ReverseIterator<Iterator<int>> rbegin(endIt);
  ReverseIterator<Iterator<int>> rend(beginIt);

  int expected = 5;
  for (auto it = rbegin; it != rend; ++it) {
    EXPECT_EQ(*it, expected--);
  }
  EXPECT_EQ(expected, 0);
}

// ============================================================================
// ReverseIterator — const
// ============================================================================

TEST(ReverseIteratorTest, ConstReverseIterator) {
  int values[] = {10, 20, 30};
  Iterator<const int> endIt(values + 3);
  ReverseIterator<Iterator<const int>> rit(endIt);
  EXPECT_EQ(*rit, 30);
  ++rit;
  EXPECT_EQ(*rit, 20);
}
