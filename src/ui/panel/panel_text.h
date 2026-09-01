#pragma once

#include <string>
#include <imgui.h>
#include <vector>

namespace UI {
enum class PanelTextAlignment { left, middle, right };

struct 
// Panel text only cares about position, size and alignment
class PanelTexts {
public:
  PanelTexts(ImVec2 m_pos, ImVec2 size, PanelTextAlignment alignment);
  void appendV(const char* text);
  void appendH(const char* text);
  void draw();

private:
  std::vector<std::string> m_texts;
  ImVec2 m_pos;
  ImVec2 m_size;
  PanelTextAlignment m_alignment;
};

} // namespace UI