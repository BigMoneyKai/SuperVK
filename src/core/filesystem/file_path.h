#pragma once

#include "core/container/string/string.hpp"
#include "core/container/string/string_view.hpp"
#include "core/define/compiler.h"
#include "core/define/types.h"
#include "core/memory/allocator.h"

class FilePath {
public:
  FilePath(Allocator *a = &default_allocator());
  FilePath(const char *src, Allocator *a = &default_allocator());

  void append(const FilePath &rel);
  StringView file_name() const;
  StringView stem() const;
  StringView extension() const;
  FilePath parent() const;
  b32 is_absolute() const;

  SV_FORCE_INLINE const String &str() const { return m_str; }
  SV_FORCE_INLINE const char *c_str() const { return m_str.c_str(); }

  b32 operator==(const FilePath &other) const;

private:
  String m_str;
};
