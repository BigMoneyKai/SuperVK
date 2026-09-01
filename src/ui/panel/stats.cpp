#include "ui/panel/stats.h"

namespace UI {
void Stats::init(const PanelInfo &info) { Panel::init(info); }

void Stats::draw(const ImRect &bounds) {
  if (!beginPanel(bounds))
    return;
  const ImGuiIO &io = ImGui::GetIO();
  ImGui::Text("FPS: %.1f", io.Framerate);
  ImGui::Text("Frame: %.2f ms", io.DeltaTime * 1000.0f);
  ImGui::Text("Window: %.0fx%.0f", io.DisplaySize.x, io.DisplaySize.y);
  endPanel();
}

void Stats::destroy() {}
} // namespace UI
