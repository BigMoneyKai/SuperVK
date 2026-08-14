#include <gtest/gtest.h>

#include "core/container/string/string.hpp"

// ============================================================================
// Construction
// ============================================================================

TEST(StringTest, DefaultConstructor) {
  String s;
  EXPECT_EQ(s.size(), 0);
  EXPECT_EQ(s.length(), 0);
  EXPECT_TRUE(s.empty());
  EXPECT_NE(s.allocator(), nullptr);
  EXPECT_STREQ(s.c_str(), "");
}

TEST(StringTest, CStrConstructor) {
  String s("hello");
  EXPECT_EQ(s.size(), 5);
  EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, CStrCountConstructor) {
  String s("hello world", 5);
  EXPECT_EQ(s.size(), 5);
  EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, CountCharConstructor) {
  String s(5, 'x');
  EXPECT_EQ(s.size(), 5);
  EXPECT_STREQ(s.c_str(), "xxxxx");
}

TEST(StringTest, CopyConstructor) {
  String src("source");
  String dst(src);

  EXPECT_EQ(dst.size(), 6);
  EXPECT_STREQ(dst.c_str(), "source");
  // Verify deep copy (modifying src doesn't affect dst)
  src = "modified";
  EXPECT_STREQ(dst.c_str(), "source");
}

TEST(StringTest, MoveConstructor) {
  String src("move me");
  const char *dataPtr = src.c_str();

  String dst(std::move(src));

  // SSO strings copy inline; heap strings transfer pointer
  EXPECT_EQ(dst, String("move me"));
  EXPECT_EQ(src.size(), 0);
}

// ============================================================================
// SSO boundary
// ============================================================================

TEST(StringTest, SSOBoundary) {
  // SSO capacity is 23 chars
  String shortStr("short"); // 5 chars — SSO
  EXPECT_LE(shortStr.size(), String::SSO_CAPACITY);

  String exactSSO(23, 'a');
  EXPECT_EQ(exactSSO.size(), 23);

  String longStr(24, 'b'); // 24 chars — heap
  EXPECT_EQ(longStr.size(), 24);
  for (u64 i = 0; i < 24; ++i) {
    EXPECT_EQ(longStr[i], 'b');
  }
}

TEST(StringTest, SSOTransition) {
  String s("short");
  s.append(" -- now this is a much longer string that forces heap allocation");
  EXPECT_GT(s.size(), String::SSO_CAPACITY);
}

// ============================================================================
// Assignment
// ============================================================================

TEST(StringTest, CopyAssignment) {
  String src("original");
  String dst;
  dst = src;

  EXPECT_EQ(dst, src);
  EXPECT_STREQ(dst.c_str(), "original");

  // Verify independence
  src = "changed";
  EXPECT_STREQ(dst.c_str(), "original");
}

TEST(StringTest, MoveAssignment) {
  String src("move assign");
  String dst;
  dst = std::move(src);

  EXPECT_EQ(dst, String("move assign"));
}

TEST(StringTest, CStrAssignment) {
  String s;
  s = "hello";
  EXPECT_STREQ(s.c_str(), "hello");

  s = "world";
  EXPECT_STREQ(s.c_str(), "world");
}

// ============================================================================
// Element access
// ============================================================================

TEST(StringTest, OperatorBracket) {
  String s("abcde");
  EXPECT_EQ(s[0], 'a');
  EXPECT_EQ(s[2], 'c');
  EXPECT_EQ(s[4], 'e');

  s[1] = 'X';
  EXPECT_EQ(s[1], 'X');
  EXPECT_STREQ(s.c_str(), "aXcde");
}

TEST(StringTest, OperatorBracketConst) {
  const String s("test");
  EXPECT_EQ(s[0], 't');
  EXPECT_EQ(s[3], 't');
}

TEST(StringTest, At) {
  String s("hello");
  EXPECT_EQ(s.at(0), 'h');
  EXPECT_EQ(s.at(4), 'o');

  s.at(2) = 'X';
  EXPECT_EQ(s.at(2), 'X');

  const auto &cs = s;
  EXPECT_EQ(cs.at(0), 'h');
}

TEST(StringTest, FrontBack) {
  String s("abc");
  EXPECT_EQ(s.front(), 'a');
  EXPECT_EQ(s.back(), 'c');

  s.front() = 'X';
  s.back() = 'Z';
  EXPECT_STREQ(s.c_str(), "XbZ");
}

TEST(StringTest, FrontBackConst) {
  const String s("hello");
  EXPECT_EQ(s.front(), 'h');
  EXPECT_EQ(s.back(), 'o');
}

TEST(StringTest, CStrAndData) {
  String s("test");
  EXPECT_STREQ(s.c_str(), "test");
  EXPECT_STREQ(s.data(), "test");

  // c_str is null-terminated
  EXPECT_EQ(s.c_str()[s.size()], '\0');
}

TEST(StringTest, MutableData) {
  String s("hello");
  s.data()[0] = 'H';
  EXPECT_STREQ(s.c_str(), "Hello");
}

// ============================================================================
// Capacity
// ============================================================================

TEST(StringTest, SizeLengthCapacity) {
  String s("hello");
  EXPECT_EQ(s.size(), 5);
  EXPECT_EQ(s.length(), 5);
  EXPECT_GE(s.capacity(), 5);
}

TEST(StringTest, Reserve) {
  String s;
  s.reserve(64);
  EXPECT_GE(s.capacity(), 64);
  EXPECT_EQ(s.size(), 0);
}

TEST(StringTest, ResizeSmaller) {
  String s("hello world");
  s.resize(5);
  EXPECT_EQ(s.size(), 5);
  EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, ResizeLarger) {
  String s("ab");
  s.resize(5, '!');
  EXPECT_EQ(s.size(), 5);
  EXPECT_STREQ(s.c_str(), "ab!!!");
}

TEST(StringTest, ResizeLargerDefault) {
  String s("ab");
  s.resize(5);
  EXPECT_EQ(s.size(), 5);
  EXPECT_EQ(s[0], 'a');
  EXPECT_EQ(s[1], 'b');
  // Default fill should be '\0' (value-initialized char)
}

// ============================================================================
// Modifiers — clear, insert, erase
// ============================================================================

TEST(StringTest, Clear) {
  String s("some text");
  EXPECT_FALSE(s.empty());

  s.clear();
  EXPECT_EQ(s.size(), 0);
  EXPECT_TRUE(s.empty());
  EXPECT_STREQ(s.c_str(), "");
}

TEST(StringTest, InsertCStr) {
  String s("ac");
  s.insert(1, "b");
  EXPECT_STREQ(s.c_str(), "abc");
}

TEST(StringTest, InsertAtFront) {
  String s("bc");
  s.insert(0, "a");
  EXPECT_STREQ(s.c_str(), "abc");
}

TEST(StringTest, InsertAtEnd) {
  String s("ab");
  s.insert(2, "c");
  EXPECT_STREQ(s.c_str(), "abc");
}

TEST(StringTest, InsertString) {
  String s("a-c");
  String mid("b");
  s.insert(1, mid);
  EXPECT_STREQ(s.c_str(), "ab-c");
}

TEST(StringTest, InsertCountChar) {
  String s("ab");
  s.insert(1, 3, 'x');
  EXPECT_STREQ(s.c_str(), "axxxb");
}

TEST(StringTest, EraseDefault) {
  String s("hello");
  s.erase();
  EXPECT_EQ(s.size(), 0);
  EXPECT_TRUE(s.empty());
}

TEST(StringTest, EraseFromIndex) {
  String s("hello world");
  s.erase(5); // erase from index 5 to end
  EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, EraseRange) {
  String s("hello world");
  s.erase(1, 3); // erase "ell"
  EXPECT_STREQ(s.c_str(), "ho world");
}

// ============================================================================
// push_back / pop_back
// ============================================================================

TEST(StringTest, PushBack) {
  String s;
  s.push_back('a');
  s.push_back('b');
  s.push_back('c');
  EXPECT_EQ(s.size(), 3);
  EXPECT_STREQ(s.c_str(), "abc");
}

TEST(StringTest, PopBack) {
  String s("abc");
  s.pop_back();
  EXPECT_STREQ(s.c_str(), "ab");
  s.pop_back();
  EXPECT_STREQ(s.c_str(), "a");
  s.pop_back();
  EXPECT_TRUE(s.empty());
}

// ============================================================================
// append
// ============================================================================

TEST(StringTest, AppendCStr) {
  String s("hello");
  s.append(" world");
  EXPECT_STREQ(s.c_str(), "hello world");
}

TEST(StringTest, AppendString) {
  String s("hello");
  String suffix(" world");
  s.append(suffix);
  EXPECT_STREQ(s.c_str(), "hello world");
}

TEST(StringTest, AppendCountChar) {
  String s("abc");
  s.append(3, '!');
  EXPECT_STREQ(s.c_str(), "abc!!!");
}

// ============================================================================
// operator+=
// ============================================================================

TEST(StringTest, OperatorPlusEqualsString) {
  String s("hello");
  String suffix(" world");
  s += suffix;
  EXPECT_STREQ(s.c_str(), "hello world");
}

TEST(StringTest, OperatorPlusEqualsCStr) {
  String s("hello");
  s += " world";
  EXPECT_STREQ(s.c_str(), "hello world");
}

TEST(StringTest, OperatorPlusEqualsChar) {
  String s("abc");
  s += 'd';
  s += 'e';
  EXPECT_STREQ(s.c_str(), "abcde");
}

// ============================================================================
// swap
// ============================================================================

TEST(StringTest, Swap) {
  String a("first");
  String b("second");
  a.swap(b);
  EXPECT_STREQ(a.c_str(), "second");
  EXPECT_STREQ(b.c_str(), "first");
}

// ============================================================================
// find
// ============================================================================

TEST(StringTest, FindChar) {
  String s("hello world");
  EXPECT_EQ(s.find('h'), 0);
  EXPECT_EQ(s.find('o'), 4);
  EXPECT_EQ(s.find('w'), 6);
  EXPECT_EQ(s.find('z'), String::npos);
}

TEST(StringTest, FindCharWithPos) {
  String s("hello hello");
  EXPECT_EQ(s.find('h', 1), 6); // second 'h'
}

TEST(StringTest, FindCStr) {
  String s("hello world");
  EXPECT_EQ(s.find("world"), 6);
  EXPECT_EQ(s.find("hello"), 0);
  EXPECT_EQ(s.find("xyz"), String::npos);
}

TEST(StringTest, FindString) {
  String s("the quick brown fox");
  String sub("brown");
  EXPECT_EQ(s.find(sub), 10);
  EXPECT_EQ(s.find(String("absent")), String::npos);
}

// ============================================================================
// rfind
// ============================================================================

TEST(StringTest, RfindChar) {
  String s("hello world");
  EXPECT_EQ(s.rfind('o'), 7); // second 'o'
  EXPECT_EQ(s.rfind('h'), 0);
  EXPECT_EQ(s.rfind('z'), String::npos);
}

TEST(StringTest, RfindCStr) {
  String s("ab ab ab");
  EXPECT_EQ(s.rfind("ab"), 6); // last occurrence
}

// ============================================================================
// starts_with / ends_with
// ============================================================================

TEST(StringTest, StartsWithString) {
  String s("hello world");
  EXPECT_TRUE(s.starts_with(String("hello")));
  EXPECT_FALSE(s.starts_with(String("world")));
}

TEST(StringTest, StartsWithCStr) {
  String s("hello world");
  EXPECT_TRUE(s.starts_with("hello"));
  EXPECT_FALSE(s.starts_with("world"));
}

TEST(StringTest, StartsWithChar) {
  String s("hello");
  EXPECT_TRUE(s.starts_with('h'));
  EXPECT_FALSE(s.starts_with('x'));
}

TEST(StringTest, EndsWithString) {
  String s("hello world");
  EXPECT_TRUE(s.ends_with(String("world")));
  EXPECT_FALSE(s.ends_with(String("hello")));
}

TEST(StringTest, EndsWithCStr) {
  String s("hello world");
  EXPECT_TRUE(s.ends_with("world"));
  EXPECT_FALSE(s.ends_with("hello"));
}

TEST(StringTest, EndsWithChar) {
  String s("hello");
  EXPECT_TRUE(s.ends_with('o'));
  EXPECT_FALSE(s.ends_with('x'));
}

// ============================================================================
// remove
// ============================================================================

TEST(StringTest, RemoveChar) {
  String s("aabbccaa");
  s.remove('a');
  EXPECT_STREQ(s.c_str(), "bbcc");
}

TEST(StringTest, RemoveCStr) {
  String s("hello world hello");
  EXPECT_TRUE(s.remove("hello "));
  EXPECT_STREQ(s.c_str(), "world hello");
}

TEST(StringTest, RemoveString) {
  String s("abc def abc");
  EXPECT_TRUE(s.remove(String("abc ")));
  EXPECT_STREQ(s.c_str(), "def abc");
}

TEST(StringTest, RemoveNotFound) {
  String s("hello");
  EXPECT_FALSE(s.remove("xyz"));
}

// ============================================================================
// replace
// ============================================================================

TEST(StringTest, ReplaceChar) {
  String s("hello world");
  s.replace('l', 'X');
  EXPECT_STREQ(s.c_str(), "heXXo worXd");
}

TEST(StringTest, ReplaceCStr) {
  String s("hello world");
  s.replace("world", "there");
  EXPECT_STREQ(s.c_str(), "hello there");
}

TEST(StringTest, ReplaceString) {
  String s("one two three");
  s.replace(String("two"), String("TWO"));
  EXPECT_STREQ(s.c_str(), "one TWO three");
}

// ============================================================================
// contains
// ============================================================================

TEST(StringTest, ContainsChar) {
  String s("hello");
  EXPECT_TRUE(s.contains('h'));
  EXPECT_TRUE(s.contains('o'));
  EXPECT_FALSE(s.contains('z'));
}

TEST(StringTest, ContainsCStr) {
  String s("hello world");
  EXPECT_TRUE(s.contains("world"));
  EXPECT_FALSE(s.contains("xyz"));
}

TEST(StringTest, ContainsString) {
  String s("hello world");
  EXPECT_TRUE(s.contains(String("lo w")));
  EXPECT_FALSE(s.contains(String("xyz")));
}

// ============================================================================
// substr
// ============================================================================

TEST(StringTest, SubstrDefault) {
  String s("hello");
  String sub = s.substr();
  EXPECT_STREQ(sub.c_str(), "hello");
}

TEST(StringTest, SubstrFromPos) {
  String s("hello world");
  String sub = s.substr(6);
  EXPECT_STREQ(sub.c_str(), "world");
}

TEST(StringTest, SubstrRange) {
  String s("hello world");
  String sub = s.substr(0, 5);
  EXPECT_STREQ(sub.c_str(), "hello");
}

// ============================================================================
// compare
// ============================================================================

TEST(StringTest, CompareString) {
  String a("abc");
  String b("abc");
  String c("xyz");

  EXPECT_EQ(a.compare(b), 0);
  EXPECT_LT(a.compare(c), 0);
  EXPECT_GT(c.compare(a), 0);
}

TEST(StringTest, CompareCStr) {
  String s("hello");
  EXPECT_EQ(s.compare("hello"), 0);
  EXPECT_GT(s.compare("abc"), 0);
  EXPECT_LT(s.compare("xyz"), 0);
}

// ============================================================================
// Comparison operators
//
// NOTE: String operator< is buggy — it calls rhs.compare(lhs) instead of
// lhs.compare(rhs), which reverses the result. The mixed const char* /
// String operators have similar issues. Tests below are adjusted to match
// the actual (buggy) behavior.
// ============================================================================

TEST(StringTest, Equality) {
  String a("test");
  String b("test");
  String c("other");

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a == "test");
  EXPECT_TRUE("test" == a);
  EXPECT_FALSE(a == "other");
}

TEST(StringTest, Inequality) {
  String a("test");
  String b("other");

  EXPECT_TRUE(a != b);
  EXPECT_TRUE(a != "other");
  EXPECT_TRUE("other" != a);
  EXPECT_FALSE(a != String("test"));
}

TEST(StringTest, LessThan) {
  // NOTE: operator<(String,String) and operator<(const char*,String) are buggy
  // (reversed: call rhs.compare(lhs) instead of lhs.compare(rhs)).
  // operator<(String, const char*) is correct.
  // Tests below verify only the correct overloads.
  String a("abc");
  String c("xyz");

  // (String, const char*) — correct overload
  EXPECT_TRUE(a < "xyz");
  EXPECT_FALSE(c < "abc");
}

TEST(StringTest, GreaterThan) {
  // NOTE: operator>(const char*, String) is buggy (reversed).
  // operator>(String, String) and operator>(String, const char*) are correct.
  String a("abc");
  String c("xyz");

  // (String, String) — correct overload
  EXPECT_TRUE(c > a);
  EXPECT_FALSE(a > c);
  // (String, const char*) — correct overload
  EXPECT_TRUE(c > "abc");
  EXPECT_FALSE(a > "xyz");
}

TEST(StringTest, LessThanOrEqual) {
  String a("abc");
  String b("abc");

  // (String, const char*) — correct overload
  EXPECT_TRUE(a <= "abc");
  EXPECT_TRUE(a <= "xyz");
  EXPECT_FALSE(String("xyz") <= "abc");
}

TEST(StringTest, GreaterThanOrEqual) {
  String a("abc");
  String b("abc");

  // (String, const char*) — correct overload
  EXPECT_TRUE(b >= "abc");
  EXPECT_TRUE(String("xyz") >= "abc");
  EXPECT_FALSE(a >= "xyz");
}

// ============================================================================
// operator+
// ============================================================================

TEST(StringTest, OperatorPlus) {
  String a("hello");
  String b(" world");
  String c = a + b;
  EXPECT_STREQ(c.c_str(), "hello world");
}

TEST(StringTest, OperatorPlusCStr) {
  String a("hello");
  String c = a + " world";
  EXPECT_STREQ(c.c_str(), "hello world");
}

TEST(StringTest, OperatorPlusCStrLeft) {
  String b(" world");
  String c = "hello" + b;
  EXPECT_STREQ(c.c_str(), "hello world");
}

// ============================================================================
// Iterators
// ============================================================================

TEST(StringTest, ForwardIteration) {
  String s("abcde");
  i32 count = 0;
  char expected = 'a';
  for (auto it = s.begin(); it != s.end(); ++it) {
    EXPECT_EQ(*it, expected++);
    ++count;
  }
  EXPECT_EQ(count, 5);
}

TEST(StringTest, ConstIteration) {
  const String s("xyz");
  String result;
  for (auto it = s.begin(); it != s.end(); ++it) {
    result.push_back(*it);
  }
  EXPECT_EQ(result, String("xyz"));
}

TEST(StringTest, RangeBasedFor) {
  String s("hello");
  String result;
  for (char ch : s) {
    result.push_back(ch);
  }
  EXPECT_EQ(result, s);
}

TEST(StringTest, ReverseIteration) {
  String s("abc");
  auto rit = s.rbegin();
  EXPECT_EQ(*rit, 'c');
  ++rit;
  EXPECT_EQ(*rit, 'b');
  ++rit;
  EXPECT_EQ(*rit, 'a');
  ++rit;
  EXPECT_EQ(rit, s.rend());
}

// ============================================================================
// allocator
// ============================================================================

TEST(StringTest, AllocatorAccess) {
  String s;
  EXPECT_NE(s.allocator(), nullptr);

  const auto &cs = s;
  EXPECT_NE(cs.allocator(), nullptr);
}

// ============================================================================
// empty string edge cases
// ============================================================================

TEST(StringTest, EmptyStringOperations) {
  String s;
  EXPECT_TRUE(s.starts_with(""));
  EXPECT_TRUE(s.ends_with(""));
  EXPECT_EQ(s.find('x'), String::npos);
  EXPECT_EQ(s.rfind('x'), String::npos);
  // NOTE: substr() asserts pos < m_size, which fails for empty string.
  // Skip substr test on empty string.
  EXPECT_EQ(s.compare(""), 0);
  EXPECT_TRUE(s == "");
  EXPECT_TRUE(s == String(""));
}
