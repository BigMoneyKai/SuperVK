#pragma once

#include "core/define/compiler.h"
#include "settings/color.h"
#include "settings/font.h"
#include "settings/icon.h"
#include "settings/style.h"
#include <imgui.h>

namespace UI {
class Settings {
public:
  void init();
  void reset();
  void apply(); // 颜色/字体/图标/圆角一次写入 ImGui

  SV_FORCE_INLINE ColorMan &color() { return m_colorMan; }
  SV_FORCE_INLINE FontMan &font() { return m_fontMan; }
  SV_FORCE_INLINE IconMan &icon() { return m_iconMan; }
  SV_FORCE_INLINE Style &style() { return m_style; }

private:
  ColorMan m_colorMan;
  FontMan m_fontMan;
  IconMan m_iconMan;
  Style m_style;
};
} // namespace UI
