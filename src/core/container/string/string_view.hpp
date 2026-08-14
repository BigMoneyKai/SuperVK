#pragma once

#include "core/container/iterator/iterator.hpp"
#include "core/container/iterator/reverse_iterator.hpp"
#include "core/container/string/string.hpp"

class StringView {
public:
  static constexpr u64 npos = ~u64{0};

  using const_iterator = Iterator<const char>;
  using const_reverse_iterator = ReverseIterator<const_iterator>;

  StringView(const char *str);
  StringView(const char *str, u64 count);
  StringView(const String &str);
  StringView(const StringView &view);

  StringView(String &&str) = delete;
  StringView &operator=(String &&other) = delete;

  StringView &operator=(const StringView &other);
  StringView &operator=(const String &other);
  StringView &operator=(const char *str);

  SV_FORCE_INLINE u64 size() const { return m_size; }
  SV_FORCE_INLINE const char *data() const { return m_data; }
  SV_FORCE_INLINE const char *c_str() const { return m_data; }

  // --- iterators ---
  SV_FORCE_INLINE const_iterator begin() const {
    return const_iterator(data());
  }
  SV_FORCE_INLINE const_iterator end() const {
    return const_iterator(data() + static_cast<addr>(m_size));
  }

  SV_FORCE_INLINE const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  SV_FORCE_INLINE const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  const char &operator[](u64 i) const;
  const char &front() const;
  const char &back() const;
  const char &at(u64 i) const;

  b32 empty() const;

  u64 find(char ch, u64 pos = 0) const;
  u64 find(const char *str, u64 pos = 0) const;
  u64 find(const StringView &view, u64 pos = 0) const;
  u64 rfind(char ch, u64 pos = npos) const;
  u64 rfind(const char *str, u64 pos = npos) const;

  b32 starts_with(const StringView &view) const;
  b32 starts_with(const char *str) const;
  b32 starts_with(char ch) const;
  b32 ends_with(const StringView &view) const;
  b32 ends_with(const char *str) const;
  b32 ends_with(char ch) const;
  String substr(u64 pos = 0, u64 count = npos) const;

  i32 compare(const StringView &view) const;
  i32 compare(const String &str) const;
  i32 compare(const char *str) const;

  // --- non-member operators ---
  friend b32 operator==(const StringView &lhs, const StringView &rhs);
  friend b32 operator==(const char *lhs, const StringView &rhs);
  friend b32 operator==(const StringView &lhs, const char *rhs);

  friend b32 operator!=(const StringView &lhs, const StringView &rhs);
  friend b32 operator!=(const char *lhs, const StringView &rhs);
  friend b32 operator!=(const StringView &lhs, const char *rhs);

  friend b32 operator<(const StringView &lhs, const StringView &rhs);
  friend b32 operator<(const char *lhs, const StringView &rhs);
  friend b32 operator<(const StringView &lhs, const char *rhs);

  friend b32 operator>(const StringView &lhs, const StringView &rhs);
  friend b32 operator>(const char *lhs, const StringView &rhs);
  friend b32 operator>(const StringView &lhs, const char *rhs);

  friend b32 operator<=(const StringView &lhs, const StringView &rhs);
  friend b32 operator<=(const char *lhs, const StringView &rhs);
  friend b32 operator<=(const StringView &lhs, const char *rhs);

  friend b32 operator>=(const StringView &lhs, const StringView &rhs);
  friend b32 operator>=(const char *lhs, const StringView &rhs);
  friend b32 operator>=(const StringView &lhs, const char *rhs);

  friend std::ostream &operator<<(std::ostream &os, const StringView &rhs);

private:
  u64 m_size;
  const char *m_data;
};

#include "string_view.inl"
