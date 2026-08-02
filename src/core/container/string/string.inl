#pragma once

#include "string.hpp"

#include "platform/memory.h"

#include <cstring>

// ============================================================================
// internal helpers
// ============================================================================

template <typename T> static SV_FORCE_INLINE T max(T a, T b) {
  return a > b ? a : b;
}

static SV_FORCE_INLINE u64 min(u64 a, u64 b) { return a < b ? a : b; }

SV_FORCE_INLINE char *String::data_ptr() {
  return m_heap ? m_storage.heap.data : m_storage.local;
}

SV_FORCE_INLINE const char *String::data_ptr() const {
  return m_heap ? m_storage.heap.data : m_storage.local;
}

inline void String::release_heap() {
  if (m_allocator)
    m_allocator->deallocate(m_storage.heap.data);
  else
    platform_aligned_free(m_storage.heap.data);
}

inline void String::grow(u64 minCapacity) {
  if (minCapacity <= capacity())
    return;

  u64 newCapacity = capacity() + (capacity() >> 1);
  if (newCapacity < minCapacity)
    newCapacity = minCapacity;
  if (newCapacity < 8)
    newCapacity = 8;

  if (!m_heap) {
    // SSO → heap
    char *newData;
    if (m_allocator)
      newData = static_cast<char *>(m_allocator->allocate(newCapacity + 1));
    else
      newData =
          static_cast<char *>(platform_aligned_alloc(newCapacity + 1, 16));

    memcpy(newData, m_storage.local, m_size);
    newData[m_size] = '\0';

    m_storage.heap.data = newData;
    m_storage.heap.capacity = newCapacity;
    m_heap = 1;
  } else {
    // heap → larger heap
    char *newData;
    if (m_allocator)
      newData = static_cast<char *>(m_allocator->allocate(newCapacity + 1));
    else
      newData =
          static_cast<char *>(platform_aligned_alloc(newCapacity + 1, 16));

    memcpy(newData, m_storage.heap.data, m_size);
    newData[m_size] = '\0';

    release_heap();

    m_storage.heap.data = newData;
    m_storage.heap.capacity = newCapacity;
  }
}

// ============================================================================
// constructors / destructor
// ============================================================================

inline String::String(Allocator *a) : m_allocator(a) {
  m_size = 0;
  m_heap = 0;
  m_storage.local[0] = '\0';
}

inline String::String(const String &other) : m_allocator(other.m_allocator) {
  m_size = other.m_size;
  m_heap = 0;

  if (other.m_heap) {
    if (m_size <= SSO_CAPACITY) {
      memcpy(m_storage.local, other.m_storage.heap.data, m_size);
      m_storage.local[m_size] = '\0';
    } else {
      u64 cap = m_size + (m_size >> 1);
      if (m_allocator)
        m_storage.heap.data =
            static_cast<char *>(m_allocator->allocate(cap + 1));
      else
        m_storage.heap.data =
            static_cast<char *>(platform_aligned_alloc(cap + 1, 16));

      m_storage.heap.capacity = cap;
      m_heap = 1;
      memcpy(m_storage.heap.data, other.m_storage.heap.data, m_size);
      m_storage.heap.data[m_size] = '\0';
    }
  } else {
    memcpy(m_storage.local, other.m_storage.local, m_size + 1);
  }
}

inline String::String(String &&other) noexcept
    : m_allocator(other.m_allocator), m_size(other.m_size),
      m_heap(other.m_heap) {
  if (m_heap) {
    m_storage.heap.data = other.m_storage.heap.data;
    m_storage.heap.capacity = other.m_storage.heap.capacity;
    other.m_storage.heap.data = nullptr;
    other.m_storage.heap.capacity = 0;
  } else {
    memcpy(m_storage.local, other.m_storage.local, m_size + 1);
  }

  other.m_size = 0;
  other.m_heap = 0;
  other.m_storage.local[0] = '\0';
}

inline String::String(const char *str, Allocator *a) : m_allocator(a) {
  u64 len = str ? strlen(str) : 0;
  m_size = len;
  m_heap = 0;

  if (len <= SSO_CAPACITY) {
    memcpy(m_storage.local, str, len);
    m_storage.local[len] = '\0';
  } else {
    u64 cap = len + (len >> 1);
    if (m_allocator)
      m_storage.heap.data = static_cast<char *>(m_allocator->allocate(cap + 1));
    else
      m_storage.heap.data =
          static_cast<char *>(platform_aligned_alloc(cap + 1, 16));

    m_storage.heap.capacity = cap;
    m_heap = 1;
    memcpy(m_storage.heap.data, str, len);
    m_storage.heap.data[len] = '\0';
  }
}

inline String::String(const char *str, u64 count, Allocator *a)
    : m_allocator(a) {
  u64 len = str ? strlen(str) : 0;
  if (count > len)
    count = len;
  m_size = count;
  m_heap = 0;

  if (count <= SSO_CAPACITY) {
    memcpy(m_storage.local, str, count);
    m_storage.local[count] = '\0';
  } else {
    u64 cap = count + (count >> 1);
    if (m_allocator)
      m_storage.heap.data = static_cast<char *>(m_allocator->allocate(cap + 1));
    else
      m_storage.heap.data =
          static_cast<char *>(platform_aligned_alloc(cap + 1, 16));

    m_storage.heap.capacity = cap;
    m_heap = 1;
    memcpy(m_storage.heap.data, str, count);
    m_storage.heap.data[count] = '\0';
  }
}

inline String::String(u64 count, char ch, Allocator *a) : m_allocator(a) {
  m_size = count;
  m_heap = 0;

  if (count <= SSO_CAPACITY) {
    memset(m_storage.local, ch, count);
    m_storage.local[count] = '\0';
  } else {
    u64 cap = count + (count >> 1);
    if (m_allocator)
      m_storage.heap.data = static_cast<char *>(m_allocator->allocate(cap + 1));
    else
      m_storage.heap.data =
          static_cast<char *>(platform_aligned_alloc(cap + 1, 16));

    m_storage.heap.capacity = cap;
    m_heap = 1;
    memset(m_storage.heap.data, ch, count);
    m_storage.heap.data[count] = '\0';
  }
}

inline String::~String() {
  if (m_heap)
    release_heap();
}

// ============================================================================
// assignment
// ============================================================================

inline String &String::operator=(const String &other) {
  if (this == &other)
    return *this;

  // free old state
  if (m_heap)
    release_heap();

  m_size = other.m_size;
  m_allocator = other.m_allocator;
  m_heap = 0;

  if (other.m_heap) {
    if (m_size <= SSO_CAPACITY) {
      memcpy(m_storage.local, other.m_storage.heap.data, m_size);
      m_storage.local[m_size] = '\0';
    } else {
      u64 cap = m_size + (m_size >> 1);
      if (m_allocator)
        m_storage.heap.data =
            static_cast<char *>(m_allocator->allocate(cap + 1));
      else
        m_storage.heap.data =
            static_cast<char *>(platform_aligned_alloc(cap + 1, 16));

      m_storage.heap.capacity = cap;
      m_heap = 1;
      memcpy(m_storage.heap.data, other.m_storage.heap.data, m_size);
      m_storage.heap.data[m_size] = '\0';
    }
  } else {
    memcpy(m_storage.local, other.m_storage.local, m_size + 1);
  }

  return *this;
}

inline String &String::operator=(String &&other) noexcept {
  if (this != &other) {
    if (m_heap)
      release_heap();

    m_size = other.m_size;
    m_heap = other.m_heap;
    m_allocator = other.m_allocator;

    if (m_heap) {
      m_storage.heap.data = other.m_storage.heap.data;
      m_storage.heap.capacity = other.m_storage.heap.capacity;
      other.m_storage.heap.data = nullptr;
      other.m_heap = 0;
    } else {
      memcpy(m_storage.local, other.m_storage.local, m_size + 1);
    }

    other.m_size = 0;
    other.m_storage.local[0] = '\0';
  }
  return *this;
}

inline String &String::operator=(const char *str) {
  u64 len = str ? strlen(str) : 0;

  // Fast path: capacity is sufficient
  if (len <= capacity()) {
    if (m_heap)
      memcpy(m_storage.heap.data, str, len);
    else
      memcpy(m_storage.local, str, len);

    data_ptr()[len] = '\0';
    m_size = len;
    return *this;
  }

  // Need reallocation
  if (m_heap)
    release_heap();

  m_size = len;
  m_heap = 0;

  if (len <= SSO_CAPACITY) {
    memcpy(m_storage.local, str, len);
    m_storage.local[len] = '\0';
  } else {
    u64 cap = len + (len >> 1);
    if (m_allocator)
      m_storage.heap.data = static_cast<char *>(m_allocator->allocate(cap + 1));
    else
      m_storage.heap.data =
          static_cast<char *>(platform_aligned_alloc(cap + 1, 16));

    m_storage.heap.capacity = cap;
    m_heap = 1;
    memcpy(m_storage.heap.data, str, len);
    m_storage.heap.data[len] = '\0';
  }

  return *this;
}

// ============================================================================
// element access
// ============================================================================

inline char &String::operator[](u64 i) {
  SV_ASSERT(i < m_size, "Index {} out of bound", i);
  return data_ptr()[i];
}

inline const char &String::operator[](u64 i) const {
  SV_ASSERT(i < m_size, "Index {} out of bound", i);
  return data_ptr()[i];
}

inline char &String::front() {
  SV_ASSERT(m_size > 0, "String is empty");
  return data_ptr()[0];
}

inline const char &String::front() const {
  SV_ASSERT(m_size > 0, "String is empty");
  return data_ptr()[0];
}

inline char &String::back() {
  SV_ASSERT(m_size > 0, "String is empty");
  return data_ptr()[m_size - 1];
}

inline const char &String::back() const {
  SV_ASSERT(m_size > 0, "String is empty");
  return data_ptr()[m_size - 1];
}

SV_FORCE_INLINE const char *String::c_str() const { return data_ptr(); }

SV_FORCE_INLINE const char *String::data() const { return data_ptr(); }

SV_FORCE_INLINE char *String::data() { return data_ptr(); }

// ============================================================================
// capacity
// ============================================================================

SV_FORCE_INLINE u64 String::size() const { return m_size; }

SV_FORCE_INLINE u64 String::length() const { return m_size; }

SV_FORCE_INLINE u64 String::capacity() const {
  return m_heap ? m_storage.heap.capacity : SSO_CAPACITY;
}

SV_FORCE_INLINE b32 String::empty() const { return m_size == 0; }

SV_FORCE_INLINE void String::reserve(u64 newCapacity) {
  if (newCapacity > capacity())
    grow(newCapacity);
}

SV_FORCE_INLINE void String::resize(u64 count) { resize(count, '\0'); }

inline void String::resize(u64 count, char ch) {
  if (count > capacity())
    grow(count);

  char *ptr = data_ptr();

  if (count > m_size)
    memset(ptr + m_size, ch, count - m_size);
  else
    ptr[count] = '\0';

  m_size = count;
}

inline void String::shrink_to_fit() {
  if (!m_heap)
    return;
  if (m_size == m_storage.heap.capacity)
    return;

  if (m_size <= SSO_CAPACITY) {
    // heap → SSO
    char *oldData = m_storage.heap.data;
    memcpy(m_storage.local, oldData, m_size);
    m_storage.local[m_size] = '\0';
    if (m_allocator)
      m_allocator->deallocate(oldData);
    else
      platform_aligned_free(oldData);
    m_heap = 0;
  } else {
    // smaller heap
    char *newData;
    if (m_allocator)
      newData = static_cast<char *>(m_allocator->allocate(m_size + 1));
    else
      newData = static_cast<char *>(platform_aligned_alloc(m_size + 1, 16));

    memcpy(newData, m_storage.heap.data, m_size);
    newData[m_size] = '\0';
    release_heap();
    m_storage.heap.data = newData;
    m_storage.heap.capacity = m_size;
  }
}

// ============================================================================
// modifiers
// ============================================================================

inline void String::clear() {
  if (m_heap)
    m_storage.heap.data[0] = '\0';
  else
    m_storage.local[0] = '\0';
  m_size = 0;
}

inline void String::insert(u64 index, const char *str) {
  SV_ASSERT(index <= m_size, "Index {} out of bound", index);

  u64 len = str ? strlen(str) : 0;
  if (len == 0)
    return;

  u64 newSize = m_size + len;
  if (newSize > capacity())
    grow(newSize);

  char *ptr = data_ptr();
  u64 tail = m_size - index;
  if (tail)
    memmove(ptr + index + len, ptr + index, tail);

  memcpy(ptr + index, str, len);
  ptr[newSize] = '\0';
  m_size = newSize;
}

SV_FORCE_INLINE void String::insert(u64 index, const String &str) {
  insert(index, str.data_ptr());
}

inline void String::insert(u64 index, u64 count, char ch) {
  SV_ASSERT(index <= m_size, "Index {} out of bound", index);
  if (count == 0)
    return;

  u64 newSize = m_size + count;
  if (newSize > capacity())
    grow(newSize);

  char *ptr = data_ptr();
  u64 tail = m_size - index;
  if (tail)
    memmove(ptr + index + count, ptr + index, tail);

  memset(ptr + index, ch, count);
  ptr[newSize] = '\0';
  m_size = newSize;
}

inline void String::erase(u64 index, u64 count) {
  SV_ASSERT(index < m_size, "Index {} out of bound", index);

  if (count == 0 || index == m_size)
    return;

  u64 eraseCount = min(count, m_size - index);
  u64 tail = m_size - (index + eraseCount);
  if (tail)
    memmove(data_ptr() + index, data_ptr() + index + eraseCount, tail);

  m_size -= eraseCount;
  data_ptr()[m_size] = '\0';
}

inline void String::push_back(char ch) {
  if (m_size + 1 > capacity())
    grow(m_size + 1);

  data_ptr()[m_size] = ch;
  data_ptr()[m_size + 1] = '\0';
  m_size++;
}

inline void String::pop_back() {
  SV_ASSERT(m_size > 0, "String is empty");

  data_ptr()[m_size - 1] = '\0';
  m_size--;
}

inline void String::append(const char *str) {
  u64 len = str ? strlen(str) : 0;
  if (len == 0)
    return;

  u64 newSize = m_size + len;
  if (newSize > capacity())
    grow(newSize);

  memcpy(data_ptr() + m_size, str, len);
  data_ptr()[newSize] = '\0';
  m_size = newSize;
}

SV_FORCE_INLINE void String::append(const String &str) {
  append(str.data_ptr());
}

inline void String::append(u64 count, char ch) {
  if (count == 0)
    return;

  u64 newSize = m_size + count;
  if (newSize > capacity())
    grow(newSize);

  memset(data_ptr() + m_size, ch, count);
  data_ptr()[newSize] = '\0';
  m_size = newSize;
}

inline void String::swap(String &other) noexcept {
  // storage
  std::swap(m_storage, other.m_storage);

  // size
  u64 tmp_size = m_size;
  m_size = other.m_size;
  other.m_size = tmp_size;

  // heap
  u64 tmp_heap = m_heap;
  m_heap = other.m_heap;
  other.m_heap = tmp_heap;

  // allocator
  std::swap(m_allocator, other.m_allocator);
}

inline char &String::at(u64 index) {
  SV_ASSERT(index < m_size, "Index {} out of bound", index);

  if (!m_heap) {
    return m_storage.local[index];
  } else {
    return m_storage.heap.data[index];
  }
}

inline const char &String::at(u64 index) const {
  SV_ASSERT(index < m_size, "Index {} out of bound", index);

  if (!m_heap) {
    return m_storage.local[index];
  } else {
    return m_storage.heap.data[index];
  }
}

SV_FORCE_INLINE String &String::operator+=(const String &str) {
  append(str);
  return *this;
}

SV_FORCE_INLINE String &String::operator+=(const char *str) {
  append(str);
  return *this;
}

SV_FORCE_INLINE String &String::operator+=(char ch) {
  push_back(ch);
  return *this;
}

// ============================================================================
// string operations
// ============================================================================

inline u64 String::find(char ch, u64 pos) const {
  if (pos >= m_size)
    return npos;
  const char *ptr = data_ptr();
  for (u64 i = pos; i < m_size; i++) {
    if (ptr[i] == ch)
      return i;
  }
  return npos;
}

inline u64 String::find(const char *str, u64 pos) const {
  if (!str || !*str)
    return pos <= m_size ? pos : npos;
  if (pos >= m_size)
    return npos;

  u64 len = strlen(str);
  const char *ptr = data_ptr();
  u64 end = m_size > len ? m_size - len : 0;

  for (u64 i = pos; i <= end; i++) {
    b32 found = SV_TRUE;
    for (u64 j = 0; j < len; j++) {
      if (ptr[i + j] != str[j]) {
        found = SV_FALSE;
        break;
      }
    }
    if (found)
      return i;
  }
  return npos;
}

SV_FORCE_INLINE u64 String::find(const String &str, u64 pos) const {
  return find(str.data_ptr(), pos);
}

inline u64 String::rfind(char ch, u64 pos) const {
  if (m_size == 0)
    return npos;

  if (m_size == 0)
    return npos;
  if (pos >= m_size)
    pos = m_size - 1;

  const char *ptr = data_ptr();
  for (i64 i = static_cast<i64>(pos); i >= 0; i--) {
    if (ptr[i] == ch)
      return static_cast<u64>(i);
  }
  return npos;
}

inline u64 String::rfind(const char *str, u64 pos) const {
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

  const char *ptr = data_ptr();
  for (i64 i = static_cast<i64>(pos); i >= 0; i--) {
    b32 found = SV_TRUE;
    for (u64 j = 0; j < len; j++) {
      if (ptr[i + j] != str[j]) {
        found = SV_FALSE;
        break;
      }
    }
    if (found)
      return static_cast<u64>(i);
  }
  return npos;
}

SV_FORCE_INLINE b32 String::starts_with(const String &str) const {
  return starts_with(str.data_ptr());
}

inline b32 String::starts_with(const char *str) const {
  if (!str)
    return SV_FALSE;
  u64 len = strlen(str);
  if (len > m_size)
    return SV_FALSE;
  return memcmp(data_ptr(), str, len) == 0;
}

SV_FORCE_INLINE b32 String::starts_with(char ch) const {
  return m_size > 0 && data_ptr()[0] == ch;
}

SV_FORCE_INLINE b32 String::ends_with(const String &str) const {
  return ends_with(str.data_ptr());
}

inline b32 String::ends_with(const char *str) const {
  if (!str)
    return SV_FALSE;
  u64 len = strlen(str);
  if (len > m_size)
    return SV_FALSE;
  return memcmp(data_ptr() + m_size - len, str, len) == 0;
}

SV_FORCE_INLINE b32 String::ends_with(char ch) const {
  return m_size > 0 && data_ptr()[m_size - 1] == ch;
}

inline void String::remove(char ch) {
  char *ptr = data_ptr();
  u64 write = 0;
  for (u64 read = 0; read < m_size; read++) {
    if (ptr[read] != ch)
      ptr[write++] = ptr[read];
  }
  m_size = write;
  ptr[m_size] = '\0';
}

inline b32 String::remove(const char *str) {
  u64 pos = find(str);
  if (pos == npos)
    return SV_FALSE;
  erase(pos, strlen(str));
  return SV_TRUE;
}

SV_FORCE_INLINE b32 String::remove(const String &str) {
  return remove(str.data_ptr());
}

inline void String::replace(char oldChar, char newChar) {
  char *ptr = data_ptr();
  for (u64 i = 0; i < m_size; i++) {
    if (ptr[i] == oldChar)
      ptr[i] = newChar;
  }
}

inline void String::replace(const char *oldStr, const char *newStr) {
  if (!oldStr || !*oldStr || !newStr)
    return;

  u64 oldLen = strlen(oldStr);
  u64 newLen = strlen(newStr);

  if (oldLen == 0)
    return;

  u64 pos = 0;
  while ((pos = find(oldStr, pos)) != npos) {
    erase(pos, oldLen);
    insert(pos, newStr);
    pos += newLen;
  }
}

SV_FORCE_INLINE void String::replace(const String &oldStr,
                                     const String &newStr) {
  replace(oldStr.data_ptr(), newStr.data_ptr());
}

SV_FORCE_INLINE b32 String::contains(char ch) const { return find(ch) != npos; }

SV_FORCE_INLINE b32 String::contains(const char *str) const {
  return find(str) != npos;
}

SV_FORCE_INLINE b32 String::contains(const String &str) const {
  return find(str) != npos;
}

inline String String::substr(u64 pos, u64 count) const {
  SV_ASSERT(pos < m_size, "Position {} out of bound", pos);

  u64 actualCount = min(count, m_size - pos);
  return String(data_ptr() + pos, actualCount, m_allocator);
}

SV_FORCE_INLINE i32 String::compare(const String &str) const {
  return compare(str.data_ptr());
}

inline i32 String::compare(const char *str) const {
  if (!str)
    return 1;

  const char *a = data_ptr();
  const char *b = str;

  while (*a && *b && *a == *b) {
    a++;
    b++;
  }

  if (*a == *b)
    return 0;
  return *a < *b ? -1 : 1;
}

// ============================================================================
// non-member operators
// ============================================================================
SV_FORCE_INLINE b32 operator==(const String &lhs, const String &rhs) {
  return rhs.compare(lhs) == 0;
}

SV_FORCE_INLINE b32 operator==(const char *lhs, const String &rhs) {
  return rhs.compare(lhs) == 0;
}

SV_FORCE_INLINE b32 operator==(const String &lhs, const char *rhs) {
  return lhs.compare(rhs) == 0;
}

SV_FORCE_INLINE b32 operator!=(const String &lhs, const String &rhs) {
  return rhs.compare(lhs) != 0;
}

SV_FORCE_INLINE b32 operator!=(const char *lhs, const String &rhs) {
  return rhs.compare(lhs) != 0;
}

SV_FORCE_INLINE b32 operator!=(const String &lhs, const char *rhs) {
  return lhs.compare(rhs) != 0;
}

SV_FORCE_INLINE b32 operator<(const String &lhs, const String &rhs) {
  return rhs.compare(lhs) < 0;
}

SV_FORCE_INLINE b32 operator<(const char *lhs, const String &rhs) {
  return rhs.compare(lhs) < 0;
}

SV_FORCE_INLINE b32 operator<(const String &lhs, const char *rhs) {
  return lhs.compare(rhs) < 0;
}

SV_FORCE_INLINE b32 operator>(const String &lhs, const String &rhs) {
  return lhs.compare(rhs) > 0;
}

SV_FORCE_INLINE b32 operator>(const char *lhs, const String &rhs) {
  return rhs.compare(lhs) > 0;
}
SV_FORCE_INLINE b32 operator>(const String &lhs, const char *rhs) {
  return lhs.compare(rhs) > 0;
}

SV_FORCE_INLINE b32 operator<=(const String &lhs, const String &rhs) {
  return rhs.compare(lhs) <= 0;
}

SV_FORCE_INLINE b32 operator<=(const char *lhs, const String &rhs) {
  return rhs.compare(lhs) <= 0;
}

SV_FORCE_INLINE b32 operator<=(const String &lhs, const char *rhs) {
  return lhs.compare(rhs) <= 0;
}

SV_FORCE_INLINE b32 operator>=(const String &lhs, const String &rhs) {
  return rhs.compare(lhs) >= 0;
}

SV_FORCE_INLINE b32 operator>=(const char *lhs, const String &rhs) {
  return rhs.compare(lhs) >= 0;
}

SV_FORCE_INLINE b32 operator>=(const String &lhs, const char *rhs) {
  return lhs.compare(rhs) >= 0;
}

SV_FORCE_INLINE String operator+(const String &lhs, const String &rhs) {
  String res(lhs);
  res.append(rhs);
  return res;
}
SV_FORCE_INLINE String operator+(const String &lhs, const char *rhs) {
  String res(lhs);
  res.append(rhs);
  return res;
}

SV_FORCE_INLINE String operator+(const char *lhs, const String &rhs) {
  String res(lhs);
  res.append(rhs);
  return res;
}

SV_FORCE_INLINE std::ostream &operator<<(std::ostream &os, const String &rhs) {
  os.write(rhs.data(), rhs.size());
  return os;
}

SV_FORCE_INLINE std::istream &operator>>(std::istream &is, String &rhs) {
  std::string tmp;
  is >> tmp;
  rhs = tmp.c_str();
  return is;
}

SV_FORCE_INLINE Allocator *String::allocator() const { return m_allocator; }
SV_FORCE_INLINE Allocator *String::allocator() { return m_allocator; }
