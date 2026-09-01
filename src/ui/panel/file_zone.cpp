#include "ui/panel/file_zone.h"

namespace UI {
void FileZone::init(const PanelInfo &info) { Panel::init(info); }

void FileZone::draw(const ImRect &bounds) {
  if (!beginPanel(bounds))
    return;
  static const char *files[] = {"AlphaBlendModeTest.glb", "red.png",
                                "mamba.png"};
  static int selected = 0;
  ImGui::Text("Resources");
  ImGui::Separator();
  for (int i = 0; i < 3; ++i)
    if (ImGui::Selectable(files[i], selected == i))
      selected = i;
  endPanel();
}

void FileZone::destroy() {}
} // namespace UI
