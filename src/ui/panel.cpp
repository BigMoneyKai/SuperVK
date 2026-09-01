#include "panel.h"

#include "imgui.h"
#include "ui/panel_info.h"

namespace UI {
void Panel::init(const PanelInfo &info) { m_info = info; }

bool Panel::beginPanel(const ImRect &bounds) {
  // 防御：必须在 NewFrame() 与 Render() 之间调用，否则直接跳过（不崩）
  ImGuiContext *ctx = ImGui::GetCurrentContext();
  if (!ctx || !ctx->WithinFrameScope)
    return SV_FALSE;

  if (!m_info.open)
    return SV_FALSE;

  // 每个面板用自己配置的圆角（Begin 期间生效，endPanel 弹出）
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, m_info.style.windowRounding);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, m_info.style.childRounding);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, m_info.style.frameRounding);
  ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, m_info.style.popupRounding);
  ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding,
                      m_info.style.scrollbarRounding);
  ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, m_info.style.grabRounding);
  ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, m_info.style.tabRounding);

  ImGui::SetNextWindowPos(bounds.Min);
  ImGui::SetNextWindowSize(bounds.GetSize());

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoBringToFrontOnFocus;
  if (!hasFlag(m_info.flag, PanelFlag::scrollable))
    flags |= ImGuiWindowFlags_NoScrollbar;
  if (hasFlag(m_info.flag, PanelFlag::transparent))
    flags |= ImGuiWindowFlags_NoBackground;

  const bool visible = ImGui::Begin(m_info.title, &m_info.open, flags);
  if (!visible)
    ImGui::PopStyleVar(7); // Begin 失败也要弹掉样式
  return visible;
}

void Panel::endPanel() {
  ImGui::End();
  ImGui::PopStyleVar(7);
}

} // namespace UI
