#include "ui/panel/top_bar.h"

namespace UI {
void TopBar::init(const PanelInfo &info) { Panel::init(info); }

void TopBar::draw(const ImRect &bounds) {
  if (!beginPanel(bounds))
    return;
  const char version[] = "SuperVK v1.0.0";
  float versionWidth = ImGui::CalcTextSize(version).x;
  float versionContentWidth = ImGui::GetContentRegionAvail().x;
  ImGui::SetCursorPosX(ImGui::GetCursorPosX()
                       + (versionContentWidth - versionWidth) * 0.5f);
  float versionHeight = ImGui::CalcTextSize(version).y;
  float versionContentHeight = ImGui::GetContentRegionAvail().y;
  ImGui::SetCursorPosY(ImGui::GetCursorPosY()
                       + (versionContentHeight - versionHeight) * 0.5f);
  ImGui::Text(version);

  endPanel();
}

void TopBar::destroy() {}
} // namespace UI
