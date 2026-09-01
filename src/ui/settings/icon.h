#pragma once

#include "core/define/compiler.h"

struct ImFont;

namespace UI {
enum class IconSize { small, medium, large, veryLarge };
enum class IconType {
  settings,
  folder,
  file,
  save,
  search,
  remove,
  play,
  pause,
  close,
  info,
  debug,
  warning,
  error,
  fatal,  

};

struct IconInfo {
  IconSize size;
  const char *path;
};

static constexpr IconInfo defaultIconInfo{
  IconSize::medium,
  "fonts/BitstromWeraNerdFont-Bold.ttf"
};

class IconMan {
public:
  void init(const IconInfo &info);
  void set(const IconInfo &info);
  void importIcons();
  void apply(); // 把图标字体加载进 ImGui atlas
  void reset();

  // 返回 Nerd Font 码位字符串（配合 font() 使用）
  const char *glyph(IconType type) const;

  SV_FORCE_INLINE const IconInfo &info() const { return m_info; }
  SV_FORCE_INLINE ImFont *font() const { return m_font; }

private:
  IconInfo m_info;
  ImFont *m_font{nullptr};
};

} // namespace UI
