#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"

struct ImFont;

namespace UI {
enum class FontSource { system, app, custom };
enum class FontSize { small, medium, large, veryLarge };

struct FontInfo {
  FontSource type;
  FontSize size;
  const char *path;
};

static constexpr FontInfo defaultFontInfo{
  FontSource::app, FontSize::medium, "resource/fonts/BitstromWeraNerdFont-Bold.ttf"};

class FontMan {
public:
  void init(const FontInfo &info);
  void set(const FontInfo &info);
  void exportIcons(b32 hasPresetIcons) {}
  void reset();
  void apply(); // 把字体加载进 ImGui atlas

  SV_FORCE_INLINE const FontInfo &info() const { return m_info; }
  SV_FORCE_INLINE ImFont *font() const { return m_font; }
  SV_FORCE_INLINE ImFont *smallFont() const { return m_smallFont; }

private:
  FontInfo m_info;
  ImFont *m_font{nullptr};
  ImFont *m_smallFont{nullptr};
};

} // namespace UI
