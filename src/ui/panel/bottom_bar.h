#pragma once

#include "imgui.h"
#include "ui/panel.h"
#include "ui/panel_info.h"

#include <vector>

namespace UI {
struct BottomBarInfo : public PanelInfo {
  // Additionals
  std::vector<PanelInfo> subPanelInfos{8};

  // constuct
  BottomBarInfo()
      : PanelInfo{.title = "Bottom bar",
                  .flag = PanelFlag::none,
                  .defaultPos = {0.f, 0.f},
                  .defaultSize = {1920.f, 50.f}} {}
};

class BottomBar : public Panel {
public:
  virtual void init(const PanelInfo &info) override;
  virtual void draw(const ImRect &bounds) override;
  virtual void destroy() override;

  void setViewportSize(ImVec2 size) { m_viewportSize = size; }

private:
  ImVec2 m_viewportSize;
};

} // namespace UI