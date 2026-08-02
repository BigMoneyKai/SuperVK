#include "string_view.hpp"

template <typename T> static SV_FORCE_INLINE T max_(T a, T b) {
  return a > b ? a : b;
}

static SV_FORCE_INLINE u64 min_(u64 a, u64 b) { return a < b ? a : b; }

SV_FORCE_INLINE StringView::StringView(const char *str) {
  m_size = strlen(str);
  m_data = str;
}

SV_FORCE_INLINE StringView::StringView(const char *str, u64 count) {
  if (count > 0) {
    m_size = count;
  } else {
    count = 0;
  }
  m_data = str;
}

SV_FORCE_INLINE StringView::StringView(const String &str) {
  m_size = str.size();
  m_data = str.data();
}

SV_FORCE_INLINE StringView::StringView(const StringView &view) {
  m_size = view.size();
  m_data = view.data();
}

SV_FORCE_INLINE StringView &StringView::operator=(const StringView &other) {
  m_data = other.data();
  m_size = other.size();

  return *this;
}

SV_FORCE_INLINE StringView &StringView::operator=(const String &other) {
  m_data = other.data();
  m_size = other.size();

  return *this;
}

SV_FORCE_INLINE StringView &StringView::operator=(const char *str) {
  m_size = strlen(str);
  m_data = str;
  return *this;
}

SV_FORCE_INLINE const char &StringView::operator[](u64 i) const {
  SV_ASSERT(i < m_size, "Index {} out of bound", i);

  return m_data[i];
}

SV_FORCE_INLINE const char &StringView::front() const { return m_data[0]; }

SV_FORCE_INLINE const char &StringView::back() const {
  return m_data[m_size - 1];
}

SV_FORCE_INLINE b32 StringView::empty() const { return m_size == 0; }

SV_FORCE_INLINE u64 StringView::find(char ch, u64 pos) const {
  if (pos >= m_size)
    return npos;
  for (u64 i = pos; i < m_size; i++) {
    if (m_data[i] == ch)
      return i;
  }
  return npos;
}

SV_FORCE_INLINE u64 StringView::find(const char *str, u64 pos) const {
  if (!str || !*str)
    return pos <= m_size ? pos : npos;
  if (pos >= m_size)
    return npos;

  u64 len = strlen(str);
  u64 end = m_size > len ? m_size - len : 0;

  for (u64 i = pos; i <= end; i++) {
    b32 found = SV_TRUE;
    for (u64 j = 0; j < len; j++) {
      if (m_data[i + j] != str[j]) {
        found = SV_FALSE;
        break;
      }
    }
    if (found)
      return i;
  }
  return npos;
}

SV_FORCE_INLINE u64 StringView::find(const StringView &view, u64 pos) const {
  return find(view.data(), pos);
}

SV_FORCE_INLINE u64 StringView::rfind(char ch, u64 pos) const {
  if (m_size == 0)
    return npos;

  if (m_size == 0)
    return npos;
  if (pos >= m_size)
    pos = m_size - 1;

  for (i64 i = static_cast<i64>(pos); i >= 0; i--) {
    if (m_data[i] == ch)
      return static_cast<u64>(i);
  }
  return npos;
}

SV_FORCE_INLINE u64 StringView::rfind(const char *str, u64 pos) const {
  if (!str || !*str)
    return pos <= m_size ? pos : npos;
  if (m_size == 0)
    return npos;

  u64 len = strlen(str);
  if (len > m_size)
    return npos;

  u64 maxStart = m_size - len;
  if (pos > maxStart)
    pos = maxStart;

  for (i64 i = static_cast<i64>(pos); i >= 0; i--) {
    b32 found = SV_TRUE;
    for (u64 j = 0; j < len; j++) {
      if (m_data[i + j] != str[j]) {
        found = SV_FALSE;
        break;
      }
    }
    if (found)
      return static_cast<u64>(i);
  }
  return npos;
}

SV_FORCE_INLINE b32 StringView::starts_with(const StringView &view) const {
  return starts_with(view.data());
}

SV_FORCE_INLINE b32 StringView::starts_with(const char *str) const {
  if (!str)
    return SV_FALSE;
  u64 len = strlen(str);
  if (len > m_size)
    return SV_FALSE;
  return memcmp(data(), str, len) == 0;
}

SV_FORCE_INLINE b32 StringView::starts_with(char ch) const {
  return front() == ch;
}

SV_FORCE_INLINE b32 StringView::ends_with(const StringView &view) const {
  return ends_with(view.data());
}

SV_FORCE_INLINE b32 StringView::ends_with(const char *str) const {
  if (!str)
    return SV_FALSE;
  u64 len = strlen(str);
  if (len > m_size)
    return SV_FALSE;
  return memcmp(data() + m_size - len, str, len) == 0;
}

SV_FORCE_INLINE b32 StringView::ends_with(char ch) const {
  return back() == ch;
}

SV_FORCE_INLINE String StringView::substr(u64 pos, u64 count) const {
  SV_ASSERT(pos < m_size, "Position {} out of bound", pos);

  u64 actualCount = min(count, m_size - pos);
  return String(m_data + pos, actualCount);
}

SV_FORCE_INLINE i32 StringView::compare(const StringView &view) const {
  return m_size - view.size();
}

SV_FORCE_INLINE i32 StringView::compare(const String &str) const {
  return m_size - str.size();
}

SV_FORCE_INLINE i32 StringView::compare(const char *str) const {
  return strcmp(str, m_data);
}

SV_FORCE_INLINE b32 operator==(const StringView &lhs, const StringView &rhs) {
  return lhs.compare(rhs) == 0;
}

SV_FORCE_INLINE b32 operator==(const char *lhs, const StringView &rhs) {
  return rhs.compare(lhs) == 0;
}

SV_FORCE_INLINE b32 operator==(const StringView &lhs, const char *rhs) {
  return lhs.compare(rhs) == 0;
}

SV_FORCE_INLINE b32 operator!=(const StringView &lhs, const StringView &rhs) {
  return lhs.compare(rhs) != 0;
}

SV_FORCE_INLINE b32 operator!=(const char *lhs, const StringView &rhs) {
  return rhs.compare(lhs) != 0;
}

SV_FORCE_INLINE b32 operator!=(const StringView &lhs, const char *rhs) {
  return lhs.compare(rhs) != 0;
}

SV_FORCE_INLINE b32 operator<(const StringView &lhs, const StringView &rhs) {
  return lhs.compare(rhs) < 0;
}

SV_FORCE_INLINE b32 operator<(const char *lhs, const StringView &rhs) {
  return rhs.compare(lhs) < 0;
}

SV_FORCE_INLINE b32 operator<(const StringView &lhs, const char *rhs) {
  return lhs.compare(rhs) < 0;
}

SV_FORCE_INLINE b32 operator>(const StringView &lhs, const StringView &rhs) {
  return lhs.compare(rhs) > 0;
}

SV_FORCE_INLINE b32 operator>(const char *lhs, const StringView &rhs) {
  return rhs.compare(lhs) > 0;
}

SV_FORCE_INLINE b32 operator>(const StringView &lhs, const char *rhs) {
  return lhs.compare(rhs) > 0;
}

SV_FORCE_INLINE b32 operator<=(const StringView &lhs, const StringView &rhs) {
  return lhs.compare(rhs) <= 0;
}

SV_FORCE_INLINE b32 operator<=(const char *lhs, const StringView &rhs) {
  return rhs.compare(lhs) <= 0;
}

SV_FORCE_INLINE b32 operator<=(const StringView &lhs, const char *rhs) {
  return lhs.compare(rhs) <= 0;
}

SV_FORCE_INLINE b32 operator>=(const StringView &lhs, const StringView &rhs) {
  return lhs.compare(rhs) >= 0;
}

SV_FORCE_INLINE b32 operator>=(const char *lhs, const StringView &rhs) {
  return rhs.compare(lhs) >= 0;
}

SV_FORCE_INLINE b32 operator>=(const StringView &lhs, const char *rhs) {
  return lhs.compare(rhs) >= 0;
}

SV_FORCE_INLINE std::ostream &operator<<(std::ostream &os,
                                         const StringView &rhs) {
  os.write(rhs.data(), rhs.size());
  return os;
}
