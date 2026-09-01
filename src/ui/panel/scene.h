#pragma once

#include "ui/panel.h"

namespace UI {
class Scene : public Panel {
public:
  virtual void init(const PanelInfo &info) override;
  virtual void draw(const ImRect& bounds) override;
  virtual void destroy() override;
};
} // namespace UI