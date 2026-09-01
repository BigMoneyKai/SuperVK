#pragma once

#include "panel_info.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace UI {

class Panel {
public:
  ~Panel() = default;
  virtual void init(const PanelInfo &info);
  virtual void draw(const ImRect &bounds) = 0;
  virtual void destroy() {}

protected:
  bool beginPanel(const ImRect &bounds);
  void endPanel();

  PanelInfo m_info;
};

} // namespace UI
