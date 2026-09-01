#pragma once

#include "ui/panel.h"

namespace UI {
class Layout;

class Splitter : public Panel {
public:
  void init(const PanelInfo &info) override;
  void draw(const ImRect &bounds) override;
  void destroy() override;

  void bind(Layout *layout, u32 index); // 1=竖(改 x) 2=横(改 y)

private:
  Layout *m_layout{nullptr};
  u32 m_index{0};
  bool m_dragging{false};
};
} // namespace UI
