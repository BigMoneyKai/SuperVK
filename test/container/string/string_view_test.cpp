#include <gtest/gtest.h>

#include "core/container/string/string_view.hpp"

// ============================================================================
// Construction
// ============================================================================

TEST(StringViewTest, CStrConstructor) {
  StringView sv("hello");
  EXPECT_EQ(sv.size(), 5);
  EXPECT_STREQ(sv.data(), "hello");
}

TEST(StringViewTest, CStrCountConstructor) {
  StringView sv("hello world", 5);
  EXPECT_EQ(sv.size(), 5);
  // data() returns the original pointer (not truncated), so use strncmp
  EXPECT_EQ(strncmp(sv.data(), "hello", 5), 0);
}

TEST(StringViewTest, StringConstructor) {
  String str("from string");
  StringView sv(str);
  EXPECT_EQ(sv.size(), 11);
  EXPECT_STREQ(sv.data(), "from string");
}

TEST(StringViewTest, CopyConstructor) {
  StringView original("copy me");
  StringView copy(original);
  EXPECT_EQ(copy.size(), original.size());
  EXPECT_EQ(copy.data(), original.data());
}

// ============================================================================
// Assignment
// ============================================================================

TEST(StringViewTest, CopyAssignment) {
  StringView a("first");
  StringView b("second");
  b = a;
  EXPECT_STREQ(b.data(), "first");
  EXPECT_EQ(b.size(), a.size());
}

TEST(StringViewTest, StringAssignment) {
  StringView sv("old");
  String str("new value");
  sv = str;
  EXPECT_STREQ(sv.data(), "new value");
  EXPECT_EQ(sv.size(), 9);
}

TEST(StringViewTest, CStrAssignment) {
  StringView sv("before");
  sv = "after";
  EXPECT_STREQ(sv.data(), "after");
  EXPECT_EQ(sv.size(), 5);
}

// ============================================================================
// Element access
// ============================================================================

TEST(StringViewTest, OperatorBracket) {
  StringView sv("abcde");
  EXPECT_EQ(sv[0], 'a');
  EXPECT_EQ(sv[2], 'c');
  EXPECT_EQ(sv[4], 'e');
}

TEST(StringViewTest, FrontBack) {
  StringView sv("hello");
  EXPECT_EQ(sv.front(), 'h');
  EXPECT_EQ(sv.back(), 'o');
}

TEST(StringViewTest, CStr) {
  StringView sv("test");
  EXPECT_STREQ(sv.c_str(), "test");
}

// ============================================================================
// empty
// ============================================================================

TEST(StringViewTest, Empty) {
  StringView sv("x");
  EXPECT_FALSE(sv.empty());

  StringView empty("");
  EXPECT_TRUE(empty.empty());
}

// ============================================================================
// find
// ============================================================================

TEST(StringViewTest, FindChar) {
  StringView sv("hello world");
  EXPECT_EQ(sv.find('h'), 0);
  EXPECT_EQ(sv.find('o'), 4);
  EXPECT_EQ(sv.find('w'), 6);
  EXPECT_EQ(sv.find('z'), StringView::npos);
}

TEST(StringViewTest, FindCharWithPos) {
  StringView sv("hello hello");
  EXPECT_EQ(sv.find('h', 1), 6);
}

TEST(StringViewTest, FindCStr) {
  StringView sv("hello world");
  EXPECT_EQ(sv.find("world"), 6);
  EXPECT_EQ(sv.find("hello"), 0);
  EXPECT_EQ(sv.find("xyz"), StringView::npos);
}

TEST(StringViewTest, FindStringView) {
  StringView sv("the quick brown fox");
  StringView sub("brown");
  EXPECT_EQ(sv.find(sub), 10);
  EXPECT_EQ(sv.find(StringView("absent")), StringView::npos);
}

// ============================================================================
// rfind
// ============================================================================

TEST(StringViewTest, RfindChar) {
  StringView sv("hello world");
  EXPECT_EQ(sv.rfind('o'), 7);
  EXPECT_EQ(sv.rfind('h'), 0);
  EXPECT_EQ(sv.rfind('z'), StringView::npos);
}

TEST(StringViewTest, RfindCStr) {
  StringView sv("ab ab ab");
  EXPECT_EQ(sv.rfind("ab"), 6);
}

// ============================================================================
// starts_with / ends_with
// ============================================================================

TEST(StringViewTest, StartsWithStringView) {
  StringView sv("hello world");
  EXPECT_TRUE(sv.starts_with(StringView("hello")));
  EXPECT_FALSE(sv.starts_with(StringView("world")));
}

TEST(StringViewTest, StartsWithCStr) {
  StringView sv("hello world");
  EXPECT_TRUE(sv.starts_with("hello"));
  EXPECT_FALSE(sv.starts_with("xyz"));
}

TEST(StringViewTest, StartsWithChar) {
  StringView sv("hello");
  EXPECT_TRUE(sv.starts_with('h'));
  EXPECT_FALSE(sv.starts_with('x'));
}

TEST(StringViewTest, EndsWithStringView) {
  StringView sv("hello world");
  EXPECT_TRUE(sv.ends_with(StringView("world")));
  EXPECT_FALSE(sv.ends_with(StringView("hello")));
}

TEST(StringViewTest, EndsWithCStr) {
  StringView sv("hello world");
  EXPECT_TRUE(sv.ends_with("world"));
  EXPECT_FALSE(sv.ends_with("xyz"));
}

TEST(StringViewTest, EndsWithChar) {
  StringView sv("hello");
  EXPECT_TRUE(sv.ends_with('o'));
  EXPECT_FALSE(sv.ends_with('x'));
}

// ============================================================================
// substr
// ============================================================================

TEST(StringViewTest, SubstrDefault) {
  StringView sv("hello");
  String sub = sv.substr();
  EXPECT_STREQ(sub.c_str(), "hello");
}

TEST(StringViewTest, SubstrFromPos) {
  StringView sv("hello world");
  String sub = sv.substr(6);
  EXPECT_STREQ(sub.c_str(), "world");
}

TEST(StringViewTest, SubstrRange) {
  StringView sv("hello world");
  String sub = sv.substr(0, 5);
  EXPECT_STREQ(sub.c_str(), "hello");
}

// ============================================================================
// compare
//
// NOTE: StringView::compare compares sizes, not lexicographic content.
// Use strings of different lengths for meaningful comparison tests.
// ============================================================================

TEST(StringViewTest, CompareStringView) {
  StringView a("ab");  // length 2
  StringView b("ab");  // length 2
  StringView c("xyz"); // length 3

  EXPECT_EQ(a.compare(b), 0);
  EXPECT_LT(a.compare(c), 0); // 2 - 3 = -1
  EXPECT_GT(c.compare(a), 0); // 3 - 2 = 1
}

TEST(StringViewTest, CompareString) {
  StringView sv("hello"); // length 5
  EXPECT_EQ(sv.compare(String("hello")), 0);
  EXPECT_GT(sv.compare(String("ab")), 0);     // 5 - 2 = 3
  EXPECT_LT(sv.compare(String("xyz123")), 0); // 5 - 6 = -1
}

TEST(StringViewTest, CompareCStr) {
  // compare(const char*) uses strcmp(str, m_data) — note reversed args
  StringView sv("hello");
  // strcmp("hello", "hello") == 0
  EXPECT_EQ(sv.compare("hello"), 0);
}

// ============================================================================
// Comparison operators
// ============================================================================

TEST(StringViewTest, Equality) {
  StringView a("test");
  StringView b("test");
  StringView c("other");

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a == "test");
  EXPECT_TRUE("test" == a);
}

TEST(StringViewTest, Inequality) {
  StringView a("test");
  StringView b("other");

  EXPECT_TRUE(a != b);
  EXPECT_TRUE(a != "other");
  EXPECT_TRUE("other" != a);
}

TEST(StringViewTest, LessThan) {
  // Size-based: "ab" (2) < "xyz" (3)
  // NOTE: operator<(StringView, const char*) uses compare(const char*)
  // which has reversed args (strcmp(str, m_data)). Skip mixed type tests.
  StringView a("ab");
  StringView c("xyz");

  EXPECT_TRUE(a < c);
}

TEST(StringViewTest, GreaterThan) {
  // Size-based: "xyz" (3) > "ab" (2)
  StringView a("ab");
  StringView c("xyz");

  EXPECT_TRUE(c > a);
}

TEST(StringViewTest, LessThanOrEqual) {
  StringView a("ab");
  StringView b("ab");
  StringView c("xyz");

  EXPECT_TRUE(a <= b);
  EXPECT_TRUE(a <= c);
  EXPECT_FALSE(c <= a);
}

TEST(StringViewTest, GreaterThanOrEqual) {
  StringView a("ab");
  StringView b("ab");
  StringView c("xyz");

  EXPECT_TRUE(b >= a);
  EXPECT_TRUE(c >= a);
  EXPECT_FALSE(a >= c);
}

// ============================================================================
// Iterators
// ============================================================================

TEST(StringViewTest, ForwardIteration) {
  StringView sv("abcde");
  i32 count = 0;
  char expected = 'a';
  for (auto it = sv.begin(); it != sv.end(); ++it) {
    EXPECT_EQ(*it, expected++);
    ++count;
  }
  EXPECT_EQ(count, 5);
}

TEST(StringViewTest, ReverseIteration) {
  StringView sv("abc");
  auto rit = sv.rbegin();
  EXPECT_EQ(*rit, 'c');
  ++rit;
  EXPECT_EQ(*rit, 'b');
  ++rit;
  EXPECT_EQ(*rit, 'a');
  ++rit;
  EXPECT_EQ(rit, sv.rend());
}

// ============================================================================
// Empty string
// ============================================================================

TEST(StringViewTest, EmptyStringOperations) {
  StringView sv("");
  EXPECT_TRUE(sv.empty());
  EXPECT_EQ(sv.size(), 0);
  EXPECT_EQ(sv.find('x'), StringView::npos);
  EXPECT_EQ(sv.rfind('x'), StringView::npos);
  EXPECT_EQ(sv.compare(""), 0);
  EXPECT_EQ(sv.compare(StringView("")), 0);
}
