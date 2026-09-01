#include "ui/settings/icon.h"

#include <imgui.h>

namespace UI {

namespace {
const char *glyphCodePoint(IconType type) {
  switch (type) {
  case IconType::settings:
    return "\uE713";
  case IconType::folder:
    return "\uF07B";
  case IconType::file:
    return "\uF15B";
  case IconType::save:
    return "\uF0C7";
  case IconType::search:
    return "\uF002";
  case IconType::remove:
    return "\uF2ED";
  case IconType::play:
    return "\uF04B";
  case IconType::pause:
    return "\uF04C";
  case IconType::close:
    return "\uF00D";
  case IconType::info:
    return "\uF129";
  case IconType::debug:
    return "\uF121";
  case IconType::warning:
    return "\uF071";
  case IconType::error:
    return "\uF057";
  case IconType::fatal:
    return "\uF057";
  }
  return "\uF128";
}
} // namespace

void IconMan::init(const IconInfo &info) { m_info = info; }

void IconMan::set(const IconInfo &info) { m_info = info; }

void IconMan::importIcons() { apply(); }

void IconMan::apply() {
  ImGuiIO &io = ImGui::GetIO();
  m_font = io.Fonts->AddFontFromFileTTF(m_info.path ? m_info.path : "", 18.f);
}

void IconMan::reset() {
  m_info = defaultIconInfo;
  m_font = nullptr;
}

const char *IconMan::glyph(IconType type) const {
  return glyphCodePoint(type);
}

} // namespace UI
