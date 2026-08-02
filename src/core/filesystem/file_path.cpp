#include "file_path.h"

FilePath::FilePath(Allocator *a) { m_str = String(a); }

FilePath::FilePath(const char *src, Allocator *a) { m_str = String(src, a); }

void FilePath::append(const FilePath &rel) {
  if (rel.is_absolute()) {
    m_str = rel.m_str;
    return;
  }
  if (m_str.empty() || m_str.back() == '/') {
    m_str.append(rel.m_str);
  } else {
    m_str.append(1, '/');
    m_str.append(rel.m_str);
  }
}

StringView FilePath::file_name() const {
  u64 pos = m_str.rfind('/');
  if (pos == String::npos) {
    return StringView(m_str.data(), m_str.size());
  }
  return StringView(m_str.data() + pos + 1, m_str.size() - pos - 1);
}

StringView FilePath::stem() const {
  StringView name = file_name();
  u64 dot = name.rfind('.');
  if (dot == StringView::npos) {
    return name;
  }
  return StringView(name.data(), dot);
}

StringView FilePath::extension() const {
  StringView name = file_name();
  u64 dot = name.rfind('.');
  if (dot == StringView::npos) {
    return StringView("");
  }
  return StringView(name.data() + dot, name.size() - dot);
}

FilePath FilePath::parent() const {
  u64 pos = m_str.rfind('/');
  if (pos == String::npos) {
    return FilePath(m_str.allocator());
  }
  if (pos == 0) {
    return FilePath("/", m_str.allocator());
  }
  return FilePath(m_str.substr(0, pos).c_str(), m_str.allocator());
}

b32 FilePath::is_absolute() const {
#if defined(SV_PLATFORM_APPLE) || defined(SV_PLATFORM_LINUX)
  if (m_str[0] == '/')
    return SV_TRUE;
  return SV_FALSE;
#elif defined(SV_PLATFORM_WINDOWS)
  if (m_str.size() >= 3) {
    // C:\xxx
    // C:/xxx
    if (((m_str[0] >= 'a' && m_str[0] <= 'z') ||
         (m_str[0] >= 'A' && m_str[0] <= 'Z')) &&
        m_str[1] == ':' && (m_str[2] == '/' || m_str[2] == '\\')) {
      return SV_TRUE;
    }
  }

  // Windows UNC path
  // \\server\share
  if (m_str.size() >= 2 && m_str[0] == '\\' && m_str[1] == '\\') {
    return SV_TRUE;
  }
  return SV_FALSE;
#else
  return SV_FALSE;
#endif
}

b32 FilePath::operator==(const FilePath &other) const {
  return m_str == other.m_str;
}
