#pragma once

#include "ui/panel.h"

namespace UI {
class Settings;

// 实时主题面板：改预设/颜色/圆角立即生效
class SettingsPanel : public Panel {
public:
  void init(const PanelInfo &info) override;
  void draw(const ImRect &bounds) override;
  void destroy() override;

  void bind(Settings *settings);

private:
  Settings *m_settings{nullptr};
};
} // namespace UI
