#include "ui/settings/style.h"

#include <imgui.h>

namespace UI {

void Style::init(const StyleInfo &info) {
  m_preset = PresetUIStyle::custom;
  m_info = info;
}

void Style::set(PresetUIStyle preset) {
  m_preset = preset;
  switch (preset) {
  case PresetUIStyle::rectangle:
    m_info = rectangleUIStyle;
    break;
  case PresetUIStyle::roundedRectangle:
    m_info = roundedRectangleUIStyle;
    break;
  case PresetUIStyle::roundedHeadRectangle:
    m_info = roundedHeadRectangleUIStyle;
    break;
  case PresetUIStyle::custom:
    break;
  }
}

void Style::set(PresetUIStyle preset, const StyleInfo &info) {
  m_preset = PresetUIStyle::custom;
  m_info = info;
  (void)preset;
}

void Style::reset() {
  m_preset = PresetUIStyle::roundedRectangle;
  m_info = roundedRectangleUIStyle;
}

void Style::apply() {
  ImGuiStyle &style = ImGui::GetStyle();

  style.WindowMinSize = ImVec2(2.f, 2.f);

  style.WindowRounding = m_info.windowRounding;
  style.ChildRounding = m_info.childRounding;
  style.FrameRounding = m_info.frameRounding;
  style.PopupRounding = m_info.popupRounding;
  style.ScrollbarRounding = m_info.scrollbarRounding;
  style.GrabRounding = m_info.grabRounding;
  style.TabRounding = m_info.tabRounding;
}

} // namespace UI
