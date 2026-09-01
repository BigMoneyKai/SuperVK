#include "panel_text.h"
#include "imgui_internal.h"

namespace UI {
PanelTexts::PanelTexts(ImVec2 pos, ImVec2 size,
                     PanelTextAlignment alignment) {
  m_pos = pos;
  m_size = size;
  m_alignment = alignment;
}

void PanelTexts::appendV(const char *text) {
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
  m_texts.push_back(text);
}

void PanelTexts::appendH(const char *text) {
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  m_texts.push_back(text);
}
void PanelTexts::draw() {
  
}

} // namespace UI