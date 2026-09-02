#include "color.h"
#include "core/debug/debugger.h"
#include "core/debug/logger_attrib.h"

namespace UI {

void ColorMan::init(const UIColorInfo &info) { m_info = info; }
void ColorMan::set(PresetUIColor color) {
  switch (color) {
  case PresetUIColor::dark:
    m_color = color;
    m_info = darkUIColor;
    break;
  case PresetUIColor::light:
    m_color = color;
    m_info = lightUIColor;
    break;
  default:
    WARNING(LogCatag::ui, "Please provide customized UI");
    break;
  }
}

void ColorMan::set(PresetUIColor color, const UIColorInfo &info) {
  switch (color) {
  case PresetUIColor::dark:
    WARNING(LogCatag::ui, "Please ensure to use PresetUIColor::custom when "
                          "customizing color theme");
    m_color = color;
    m_info = darkUIColor;
    break;
  case PresetUIColor::light:
    WARNING(LogCatag::ui, "Please ensure to use PresetUIColor::custom when "
                          "customizing color theme");
    m_color = color;
    m_info = lightUIColor;
    break;
  case PresetUIColor::custom:
    m_color = color;
    m_info = info;
    break;
  default:
    WARNING(
      LogCatag::ui,
      "Preset UI color doesn't contain it, please add it into \"color.h\"");
    m_color = color;
    m_info = info;
  }
}

void ColorMan::reset() {
  m_color = PresetUIColor::dark;
  m_info = darkUIColor;
}

void ColorMan::apply() {
  ImGuiStyle &style = ImGui::GetStyle();

  // Window / Popup
  style.Colors[ImGuiCol_WindowBg] = m_info.windowBg;
  style.Colors[ImGuiCol_ChildBg] = m_info.childBg;
  style.Colors[ImGuiCol_PopupBg] = m_info.popupBg;

  // Border
  style.Colors[ImGuiCol_Border] = m_info.border;
  style.Colors[ImGuiCol_BorderShadow] = m_info.borderShadow;

  // Frame（输入框/滑条轨道）
  style.Colors[ImGuiCol_FrameBg] = m_info.frameBg;
  style.Colors[ImGuiCol_FrameBgHovered] = m_info.frameBgHovered;
  style.Colors[ImGuiCol_FrameBgActive] = m_info.frameBgActive;

  // Button
  style.Colors[ImGuiCol_Button] = m_info.button;
  style.Colors[ImGuiCol_ButtonHovered] = m_info.buttonHovered;
  style.Colors[ImGuiCol_ButtonActive] = m_info.buttonActive;

  // Header
  style.Colors[ImGuiCol_Header] = m_info.header;
  style.Colors[ImGuiCol_HeaderHovered] = m_info.headerHovered;
  style.Colors[ImGuiCol_HeaderActive] = m_info.headerActive;

  // Text
  style.Colors[ImGuiCol_Text] = m_info.text;
  style.Colors[ImGuiCol_TextDisabled] = m_info.textDisabled;
  style.Colors[ImGuiCol_TextSelectedBg] = m_info.textSelectedBg;

  // Check / Radio
  style.Colors[ImGuiCol_CheckMark] = m_info.checkMark;

  // Slider
  style.Colors[ImGuiCol_SliderGrab] = m_info.sliderGrab;
  style.Colors[ImGuiCol_SliderGrabActive] = m_info.sliderGrabActive;

  // Scrollbar
  style.Colors[ImGuiCol_ScrollbarBg] = m_info.scrollbarBg;
  style.Colors[ImGuiCol_ScrollbarGrab] = m_info.scrollbarGrab;
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = m_info.scrollbarGrabHovered;
  style.Colors[ImGuiCol_ScrollbarGrabActive] = m_info.scrollbarGrabActive;

  // Separator
  style.Colors[ImGuiCol_Separator] = m_info.separator;
  style.Colors[ImGuiCol_SeparatorHovered] = m_info.separatorHovered;
  style.Colors[ImGuiCol_SeparatorActive] = m_info.separatorActive;

  // Resize Grip
  style.Colors[ImGuiCol_ResizeGrip] = m_info.resizeGrip;
  style.Colors[ImGuiCol_ResizeGripHovered] = m_info.resizeGripHovered;
  style.Colors[ImGuiCol_ResizeGripActive] = m_info.resizeGripActive;

  // Tab
  style.Colors[ImGuiCol_Tab] = m_info.tab;
  style.Colors[ImGuiCol_TabHovered] = m_info.tabHovered;
  style.Colors[ImGuiCol_TabSelected] = m_info.tabSelected;
  style.Colors[ImGuiCol_TabSelectedOverline] = m_info.tabSelectedOverline;
  style.Colors[ImGuiCol_TabDimmed] = m_info.tabDimmed;
  style.Colors[ImGuiCol_TabDimmedSelected] = m_info.tabDimmedSelected;
  style.Colors[ImGuiCol_TabDimmedSelectedOverline] =
      m_info.tabDimmedSelectedOverline;

  // Table
  style.Colors[ImGuiCol_TableHeaderBg] = m_info.tableHeaderBg;
  style.Colors[ImGuiCol_TableBorderStrong] = m_info.tableBorderStrong;
  style.Colors[ImGuiCol_TableBorderLight] = m_info.tableBorderLight;
  style.Colors[ImGuiCol_TableRowBg] = m_info.tableRowBg;
  style.Colors[ImGuiCol_TableRowBgAlt] = m_info.tableRowBgAlt;

  // Plot
  style.Colors[ImGuiCol_PlotLines] = m_info.plotLines;
  style.Colors[ImGuiCol_PlotLinesHovered] = m_info.plotLinesHovered;
  style.Colors[ImGuiCol_PlotHistogram] = m_info.plotHistogram;
  style.Colors[ImGuiCol_PlotHistogramHovered] = m_info.plotHistogramHovered;

  // Navigation / Modal
  style.Colors[ImGuiCol_NavHighlight] = m_info.navHighlight;
  style.Colors[ImGuiCol_NavWindowingHighlight] = m_info.navWindowingHighlight;
  style.Colors[ImGuiCol_NavWindowingDimBg] = m_info.navWindowingDimBg;
  style.Colors[ImGuiCol_ModalWindowDimBg] = m_info.modalWindowDimBg;

  // Drag & Drop
  style.Colors[ImGuiCol_DragDropTarget] = m_info.dragDropTarget;

  // 注：本版 ImGui 无 TooltipBg 颜色；inputBg/accent/status
  //     是应用语义色（如控制台用），不直接映射 ImGui
}

} // namespace UI
