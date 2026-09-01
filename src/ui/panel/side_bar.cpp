#include "ui/panel/side_bar.h"
#include "ui/define.h"
#include <vector>

namespace UI {
void SideBar::init(const PanelInfo &info) { Panel::init(info); }

void SideBar::draw(const ImRect &bounds) {
  if (!beginPanel(bounds))
    return;
  static int tool = 0;
  static std::vector<const char*> icons = {Icon::file, Icon::search, Icon::debug};
  for (int i = 0; i < icons.size(); ++i)
    if (ImGui::Selectable(icons[i], tool == i))
      tool = i;
  endPanel();
}

void SideBar::destroy() {}
} // namespace UI
