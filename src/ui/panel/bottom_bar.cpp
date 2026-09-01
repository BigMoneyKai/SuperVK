#pragma once

#include "bottom_bar.h"
#include "imgui.h"

namespace UI {
void BottomBar::init(const PanelInfo &info) {
  Panel::init(info);
  m_viewportSize = {0.f, 0.f};
}

void BottomBar::draw(const ImRect &bounds) {
  if (!beginPanel(bounds))
    return;

  const ImGuiIO &io = ImGui::GetIO();
  const f32 fps = io.Framerate;
  const ImVec4 color = fps < 30.f ? ImVec4(1.f, 0.3f, 0.3f, 1.f)
                                  : ImGui::GetStyleColorVec4(ImGuiCol_Text);

  // FPS
  const char *fpsText = "FPS: %.1f  (%.2f ms)";
  float FPSHeight = ImGui::CalcTextSize(fpsText).y;
  float FPSContentHeight = ImGui::GetContentRegionAvail().y;
  ImGui::SetCursorPosY(ImGui::GetCursorPosY()
                       + (FPSContentHeight - FPSHeight) * 0.5f);
  ImGui::TextColored(color, fpsText, fps, io.DeltaTime * 1000.0f);

  // Viewport
  ImGui::SameLine();
  const char *viewportText = "Viewport: %.0fx%.0f";
  float viewportWidth = ImGui::CalcTextSize(viewportText).x;
  float viewportContentWidth = ImGui::GetContentRegionAvail().x;
  ImGui::SetCursorPosX(ImGui::GetCursorPosX()
                       + (viewportContentWidth - viewportWidth));
  float viewportHeight = ImGui::CalcTextSize(viewportText).y;
  float viewportContentHeight = ImGui::GetContentRegionAvail().y;
  ImGui::SetCursorPosY(ImGui::GetCursorPosY()
                       + (viewportContentHeight - viewportHeight) * 0.5f);
  ImGui::TextColored(color, viewportText, m_viewportSize.x, m_viewportSize.y);

  // Window info
  ImGui::SameLine();
  const char *windowText = "Window: %.0fx%.0f";
  float windowWidth = ImGui::CalcTextSize(windowText).x;
  float windowContentWidth = ImGui::GetContentRegionAvail().x;
  ImGui::SetCursorPosX(ImGui::GetCursorPosX()
                       + (windowContentWidth - windowWidth - viewportWidth));
  float windowHeight = ImGui::CalcTextSize(windowText).y;
  float windowContentHeight = ImGui::GetContentRegionAvail().y;
  ImGui::SetCursorPosY(ImGui::GetCursorPosY()
                       + (windowContentHeight - windowHeight) * 0.5f);
  ImGui::TextColored(color, windowText, io.DisplaySize.x, io.DisplaySize.y);

  endPanel();
}

void BottomBar::destroy() {}

} // namespace UI
