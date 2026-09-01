#include "ui/settings/font.h"
#include "core/debug/debugger.h"

#include <imgui.h>

namespace UI {

namespace {
f32 fontPixelSize(FontSize size) {
  switch (size) {
  case FontSize::small:
    return 10.f;
  case FontSize::medium:
    return 14.f;
  case FontSize::large:
    return 16.f;
  case FontSize::veryLarge:
    return 20.f;
  }
  return 14.f;
}
} // namespace

void FontMan::init(const FontInfo &info) {
  m_info = info;
}

void FontMan::set(const FontInfo &info) {
  m_info = info;
}

void FontMan::reset() {
  m_info = defaultFontInfo;
  m_font = nullptr;
  m_smallFont = nullptr;
}

void FontMan::apply() {
  ImGuiIO &io = ImGui::GetIO();
  const f32 px = fontPixelSize(m_info.size);

  const auto loadOrFallback = [&io](const char *path, f32 size) -> ImFont * {
    ImFont *f = io.Fonts->AddFontFromFileTTF(path, size);
    if (!f) {
      WARNING(LogCatag::ui, "Font not found: {}, fallback to system font",
              path ? path : "?");
      f = io.Fonts->AddFontFromFileTTF(
          "/System/Library/Fonts/Supplemental/Arial Black.ttf", size);
    }
    return f;
  };

  m_font = loadOrFallback(m_info.path, px);
  m_smallFont = loadOrFallback(m_info.path, px * 0.7f); // 细条（顶栏/底栏）用

  if (!m_font || !m_smallFont) {
    WARNING(LogCatag::ui, "Font not found: {}, fallback to system font",
            m_info.path ? m_info.path : "?");
  }
}
} // namespace UI
