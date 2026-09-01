#include "ui/panel/settings_panel.h"
#include "ui/settings.h"

namespace UI {

void SettingsPanel::init(const PanelInfo &info) { Panel::init(info); }

void SettingsPanel::bind(Settings *settings) { m_settings = settings; }

void SettingsPanel::draw(const ImRect &bounds) {
  if (!beginPanel(bounds) || !m_settings)
    return;

  Settings &s = *m_settings;

  // ---- 预设 ----
  static const char *colorNames[] = {"Dark", "Light", "Custom"};
  int colorIdx = static_cast<int>(s.color().color());
  if (colorIdx > 2)
    colorIdx = 0;
  if (ImGui::Combo("Theme", &colorIdx, colorNames, 3)) {
    s.color().set(static_cast<PresetUIColor>(colorIdx));
    s.apply();
  }

  static const char *styleNames[] = {"Rectangle", "Rounded", "Rounded Head"};
  int styleIdx = static_cast<int>(s.style().preset());
  if (styleIdx > 2)
    styleIdx = 1;
  if (ImGui::Combo("Style", &styleIdx, styleNames, 3)) {
    s.style().set(static_cast<PresetUIStyle>(styleIdx));
    s.apply();
  }
  ImGui::Separator();

  // ---- 颜色微调（改完立即 apply）----
  if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen)) {
    UIColorInfo info = s.color().info();
    bool changed = false;
    changed |= ImGui::ColorEdit4("Window BG", &info.windowBg.x);
    changed |= ImGui::ColorEdit4("Child BG", &info.childBg.x);
    changed |= ImGui::ColorEdit4("Button", &info.button.x);
    changed |= ImGui::ColorEdit4("Button Hover", &info.buttonHovered.x);
    changed |= ImGui::ColorEdit4("Header", &info.header.x);
    changed |= ImGui::ColorEdit4("Text", &info.text.x);
    if (changed) {
      s.color().set(PresetUIColor::custom, info);
      s.apply();
    }
  }

  // ---- 圆角 ----
  if (ImGui::CollapsingHeader("Rounding")) {
    StyleInfo style = s.style().info();
    bool changed = false;
    changed |= ImGui::SliderFloat("Window", &style.windowRounding, 0.f, 16.f);
    changed |= ImGui::SliderFloat("Frame", &style.frameRounding, 0.f, 16.f);
    changed |= ImGui::SliderFloat("Popup", &style.popupRounding, 0.f, 16.f);
    changed |= ImGui::SliderFloat("Grab", &style.grabRounding, 0.f, 16.f);
    if (changed) {
      s.style().set(PresetUIStyle::custom, style);
      s.apply();
    }
  }

  if (ImGui::Button("Reset")) {
    s.reset();
    s.apply();
  }

  endPanel();
}

void SettingsPanel::destroy() {}

} // namespace UI
