#include "ui/panel/splitter.h"
#include "ui/layout.h"

namespace UI {
void Splitter::init(const PanelInfo &info) { Panel::init(info); }

void Splitter::bind(Layout *layout, u32 index) {
  m_layout = layout;
  m_index = index;
}

void Splitter::draw(const ImRect &bounds) {
  if (!m_layout)
    return;

  const bool vertical = m_index == 1;
  ImGuiIO &io = ImGui::GetIO();

  // 视觉：在分隔边界画一条线（前景层，永远可见）
  ImDrawList *dl = ImGui::GetForegroundDrawList();
  const ImVec2 a = vertical ? ImVec2(bounds.Min.x, bounds.Min.y)
                            : ImVec2(bounds.Min.x, bounds.Min.y);
  const ImVec2 b = vertical ? ImVec2(bounds.Min.x, bounds.Max.y)
                            : ImVec2(bounds.Max.x, bounds.Min.y);
  dl->AddLine(a, b, IM_COL32(255, 255, 255, 50), 1.f);

  // 命中区加宽，方便抓住细条
  ImRect hit = bounds;
  hit.Expand(vertical ? ImVec2(4.f, 0.f) : ImVec2(0.f, 4.f));

  // 交互：手动命中测试 + 拖拽状态跟踪（不依赖窗口）
  const bool hovered = hit.Contains(io.MousePos);
  if (hovered || m_dragging)
    ImGui::SetMouseCursor(vertical ? ImGuiMouseCursor_ResizeEW
                                   : ImGuiMouseCursor_ResizeNS);

  if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {

    m_dragging = true;
  }

  if (m_dragging && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
    LayoutConfig &cfg = m_layout->config();
    const float delta = vertical ? io.MouseDelta.x : io.MouseDelta.y;
    if (vertical)
      cfg.splitter1Pos.x += delta;
    else
      cfg.splitter2Pos.y += delta;
  } else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
    m_dragging = false;
  }

}

void Splitter::destroy() {}
} // namespace UI
