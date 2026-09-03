#pragma once

#include "iterator.hpp"
#include "reverse_iterator.hpp"
#include "core/define/compiler.h"
#include "core/define/types.h"
#include "core/memory/allocator.h"

#include <iostream>

class String {
public:
  static constexpr u64 npos = ~u64{0};
  static constexpr u64 SSO_CAPACITY = 63;

  using iterator = StringIterator<char>;
  using const_iterator = StringIterator<const char>;
  using reverse_iterator = StringReverseIterator<iterator>;
  using const_reverse_iterator = StringReverseIterator<const_iterator>;

  // --- constructors ---
  explicit String(Allocator *a = &default_allocator());
  String(const String &other);
  String(String &&other) noexcept;
  String(const char *str, Allocator *a = &default_allocator());
  String(const char *str, u64 count, Allocator *a = &default_allocator());
  String(u64 count, char ch, Allocator *a = &default_allocator());

  ~String();

  // --- assignment ---
  String &operator=(const String &other);
  String &operator=(String &&other) noexcept;
  String &operator=(const char *str);

  // --- iterators ---
  SV_FORCE_INLINE iterator begin() { return iterator(data_ptr()); }
  SV_FORCE_INLINE const_iterator begin() const {
    return const_iterator(data_ptr());
  }
  SV_FORCE_INLINE iterator end() {
    return iterator(data_ptr() + static_cast<addr>(m_size));
  }
  SV_FORCE_INLINE const_iterator end() const {
    return const_iterator(data_ptr() + static_cast<addr>(m_size));
  }

  SV_FORCE_INLINE reverse_iterator rbegin() { return reverse_iterator(end()); }
  SV_FORCE_INLINE const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  SV_FORCE_INLINE reverse_iterator rend() { return reverse_iterator(begin()); }
  SV_FORCE_INLINE const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  // --- element access ---
  char &operator[](u64 i);
  const char &operator[](u64 i) const;
  char &front();
  const char &front() const;
  char &back();
  const char &back() const;
  const char *c_str() const;
  const char *data() const;
  char *data();

  // --- capacity ---
  u64 size() const;
  u64 length() const;
  u64 capacity() const;
  b32 empty() const;

  // --- size and capacity operation ---
  void reserve(u64 newCapacity);
  void resize(u64 count);
  void resize(u64 count, char ch);
  void shrink_to_fit();

  // --- modifiers ---
  void clear();
  void insert(u64 index, const char *str);
  void insert(u64 index, const String &str);
  void insert(u64 index, u64 count, char ch);
  void erase(u64 index = 0, u64 count = npos);
  void push_back(char ch);
  void pop_back();
  void append(const char *str);
  void append(const String &str);
  void append(u64 count, char ch);
  void swap(String &other) noexcept;
  char &at(u64 index);
  const char &at(u64 index) const;
  String &operator+=(const String &str);
  String &operator+=(const char *str);
  String &operator+=(char ch);

  // --- string operations ---
  u64 find(char ch, u64 pos = 0) const;
  u64 find(const char *str, u64 pos = 0) const;
  u64 find(const String &str, u64 pos = 0) const;
  u64 rfind(char ch, u64 pos = npos) const;
  u64 rfind(const char *str, u64 pos = npos) const;

  b32 starts_with(const String &str) const;
  b32 starts_with(const char *str) const;
  b32 starts_with(char ch) const;
  b32 ends_with(const String &str) const;
  b32 ends_with(const char *str) const;
  b32 ends_with(char ch) const;

  void remove(char ch);
  b32 remove(const char *str);
  b32 remove(const String &str);
  void replace(char oldChar, char newChar);
  void replace(const char *oldStr, const char *newStr);
  void replace(const String &oldStr, const String &newStr);
  b32 contains(char ch) const;
  b32 contains(const char *str) const;
  b32 contains(const String &str) const;

  String substr(u64 pos = 0, u64 count = npos) const;

  i32 compare(const String &str) const;
  i32 compare(const char *str) const;

  // --- non-member operators ---
  friend b32 operator==(const String &lhs, const String &rhs);
  friend b32 operator==(const char *lhs, const String &rhs);
  friend b32 operator==(const String &lhs, const char *rhs);

  friend b32 operator!=(const String &lhs, const String &rhs);
  friend b32 operator!=(const char *lhs, const String &rhs);
  friend b32 operator!=(const String &lhs, const char *rhs);

  friend b32 operator<(const String &lhs, const String &rhs);
  friend b32 operator<(const char *lhs, const String &rhs);
  friend b32 operator<(const String &lhs, const char *rhs);

  friend b32 operator>(const String &lhs, const String &rhs);
  friend b32 operator>(const char *lhs, const String &rhs);
  friend b32 operator>(const String &lhs, const char *rhs);

  friend b32 operator<=(const String &lhs, const String &rhs);
  friend b32 operator<=(const char *lhs, const String &rhs);
  friend b32 operator<=(const String &lhs, const char *rhs);

  friend b32 operator>=(const String &lhs, const String &rhs);
  friend b32 operator>=(const char *lhs, const String &rhs);
  friend b32 operator>=(const String &lhs, const char *rhs);

  friend String operator+(const String &lhs, const String &rhs);
  friend String operator+(const String &lhs, const char *rhs);
  friend String operator+(const char *lhs, const String &rhs);

  friend std::ostream &operator<<(std::ostream &os, const String &rhs);
  friend std::istream &operator>>(std::istream &is, String &rhs);

  Allocator *allocator() const;
  Allocator *allocator();

private:
  // Storage layout (80 bytes total):
  //   union: 64 bytes (SSO buffer 63 + padding, or heap data/control)
  //   m_size + m_heap flag: 8 bytes (63-bit size + 1-bit flag)
  //   m_allocator: 8 bytes
  //
  // SSO mode (m_heap == 0):
  //   m_storage.local[0..m_size] = string, null-terminated
  //
  // Heap mode (m_heap == 1):
  //   m_storage.heap.data   = heap-allocated buffer
  //   m_storage.heap.capacity = buffer capacity

  union Storage {
    char local[SSO_CAPACITY + 1];
    struct {
      char *data;
      u64 capacity;
    } heap;
  } m_storage;

  u64 m_size : 63;
  u64 m_heap : 1;
  Allocator *m_allocator;

  // --- internal helpers ---
  char *data_ptr();
  const char *data_ptr() const;

  void release_heap();
  void grow(u64 minCapacity);
};

#include "string.inl"
