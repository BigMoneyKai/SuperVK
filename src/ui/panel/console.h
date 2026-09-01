#pragma once

#include "core/container/ring_buffer/ring_buffer.hpp"
#include "core/container/string/string.hpp"
#include "core/debug/logger_attrib.h"
#include "ui/panel.h"

namespace UI {
struct ConsoleInfo : public PanelInfo {
  // constuct
  ConsoleInfo()
      : PanelInfo{.title = "Console",
                  .defaultPos = {360.f, 50.f},
                  .defaultSize = {1000.f, 360.f}} {}
};

class Console : public Panel {
public:
  void init(const PanelInfo &info) override;
  void update(); // 每帧从 Logger 增量拉消息
  void draw(const ImRect &bounds) override;
  void destroy() override;

private:
  struct Entry {
    u64 id;
    LogLevel level;
    String text;
  };
  static constexpr u64 kMaxLines = 1000;

  RingBuffer<Entry> m_entries;
  u64 m_lastId{0};
  bool m_autoScroll{true};
  bool m_showDebug{true};
  bool m_showInfo{true};
  bool m_showWarning{true};
  bool m_showError{true};
};

} // namespace UI
