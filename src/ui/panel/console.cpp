#include "ui/panel/console.h"
#include "core/debug/logger.h"

namespace UI {

namespace {
ImVec4 levelColor(LogLevel level) {
  switch (level) {
  case LogLevel::trace:
  case LogLevel::debug:
    return ImVec4(0.55f, 0.55f, 0.60f, 1.0f);
  case LogLevel::info:
    return ImVec4(0.90f, 0.90f, 0.92f, 1.0f);
  case LogLevel::warning:
    return ImVec4(1.00f, 0.80f, 0.25f, 1.0f);
  case LogLevel::error:
  case LogLevel::fatal:
    return ImVec4(1.00f, 0.35f, 0.35f, 1.0f);
  }
  return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

bool levelVisible(LogLevel level, bool dbg, bool info, bool warn, bool err) {
  switch (level) {
  case LogLevel::trace:
  case LogLevel::debug:
    return dbg;
  case LogLevel::info:
    return info;
  case LogLevel::warning:
    return warn;
  case LogLevel::error:
  case LogLevel::fatal:
    return err;
  }
  return true;
}
} // namespace

void Console::init(const PanelInfo &info) {
  Panel::init(info);
  m_lastId = Logger::latestId(); // 只显示初始化之后的日志
}

void Console::update() {
  Logger::readMessages(
    m_lastId,
    [](const LogMsg &msg, void *userData) {
      auto *self = static_cast<Console *>(userData);
      if (self->m_entries.size() >= Console::kMaxLines)
        self->m_entries.pop_front();
      self->m_entries.push_back(Entry{msg.id, msg.level, msg.msg});
      self->m_lastId = msg.id + 1; // 下次从下一条开始，避免重复拉最后一条
    },
    this);
}

void Console::draw(const ImRect &bounds) {
  if (!beginPanel(bounds))
    return;

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
  ImGui::Checkbox("Debug", &m_showDebug);
  ImGui::SameLine();
  ImGui::Checkbox("Info", &m_showInfo);
  ImGui::SameLine();
  ImGui::Checkbox("Warn", &m_showWarning);
  ImGui::SameLine();
  ImGui::Checkbox("Error", &m_showError);
  ImGui::SameLine();
  if (ImGui::Button("Clear")) {
    m_entries.clear();
    m_lastId = Logger::latestId();
  }
  ImGui::SameLine();
  ImGui::Checkbox("AutoScroll", &m_autoScroll);
  ImGui::PopStyleVar();
  ImGui::Separator();

  ImGui::BeginChild("LogScroll", ImVec2(0.f, 0.f), false,
                    ImGuiWindowFlags_HorizontalScrollbar);
  for (const auto &e : m_entries) {
    if (!levelVisible(e.level, m_showDebug, m_showInfo, m_showWarning,
                      m_showError))
      continue;
    ImGui::TextColored(levelColor(e.level), "%s", e.text.c_str());
  }
  if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 8.f)
    ImGui::SetScrollHereY(1.0f);
  ImGui::EndChild();

  endPanel();
}

void Console::destroy() {}
} // namespace UI
