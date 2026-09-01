#pragma once

#include "core/define/compiler.h"
#include <imgui.h>

namespace UI {
// ======== Color ========
enum class PresetUIColor { dark, light, custom };

struct UIColorInfo {
  // ======== Window ========

  ImVec4 windowBg;
  ImVec4 childBg;
  ImVec4 popupBg;

  // ======== Border ========

  ImVec4 border;
  ImVec4 borderShadow;

  // ======== Frame ========

  ImVec4 frameBg;
  ImVec4 frameBgHovered;
  ImVec4 frameBgActive;

  // ======== Button ========

  ImVec4 button;
  ImVec4 buttonHovered;
  ImVec4 buttonActive;

  // ======== Header ========

  ImVec4 header;
  ImVec4 headerHovered;
  ImVec4 headerActive;

  // ======== Text ========

  ImVec4 text;
  ImVec4 textDisabled;
  ImVec4 textSelectedBg;

  // ======== Check / Radio ========

  ImVec4 checkMark;

  // ======== Slider ========

  ImVec4 sliderGrab;
  ImVec4 sliderGrabActive;

  // ======== Scrollbar ========

  ImVec4 scrollbarBg;
  ImVec4 scrollbarGrab;
  ImVec4 scrollbarGrabHovered;
  ImVec4 scrollbarGrabActive;

  // ======== Separator ========

  ImVec4 separator;
  ImVec4 separatorHovered;
  ImVec4 separatorActive;

  // ======== Resize Grip ========

  ImVec4 resizeGrip;
  ImVec4 resizeGripHovered;
  ImVec4 resizeGripActive;

  // ======== Tab ========

  ImVec4 tab;
  ImVec4 tabHovered;
  ImVec4 tabSelected;
  ImVec4 tabSelectedOverline;
  ImVec4 tabDimmed;
  ImVec4 tabDimmedSelected;
  ImVec4 tabDimmedSelectedOverline;

  // ======== Docking ========

  ImVec4 dockingPreview;
  ImVec4 dockingEmptyBg;

  // ======== Table ========

  ImVec4 tableHeaderBg;
  ImVec4 tableBorderStrong;
  ImVec4 tableBorderLight;
  ImVec4 tableRowBg;
  ImVec4 tableRowBgAlt;

  // ======== Plot ========

  ImVec4 plotLines;
  ImVec4 plotLinesHovered;
  ImVec4 plotHistogram;
  ImVec4 plotHistogramHovered;

  // ======== Navigation ========

  ImVec4 navHighlight;
  ImVec4 navWindowingHighlight;
  ImVec4 navWindowingDimBg;
  ImVec4 modalWindowDimBg;

  // ======== Drag & Drop ========

  ImVec4 dragDropTarget;

  // ======== Input / Selection ========

  ImVec4 inputBg;
  ImVec4 inputHovered;
  ImVec4 inputActive;

  // ======== Tooltip ========

  ImVec4 tooltipBg;

  // ======== Accent ========

  ImVec4 accent;
  ImVec4 accentHovered;
  ImVec4 accentActive;

  // ======== Status ========

  ImVec4 success;
  ImVec4 warning;
  ImVec4 error;
  ImVec4 info;
};

static constexpr UIColorInfo darkUIColor{

  // ======== Window ========

  {0.120f, 0.120f, 0.120f, 1.0f},  // windowBg
  {0.075f, 0.080f, 0.090f, 1.0f}, // childBg
  {0.065f, 0.070f, 0.080f, 1.0f}, // popupBg

  // ======== Border ========

  {0.22f, 0.23f, 0.25f, 1.0f},  // border
  {0.00f, 0.00f, 0.00f, 0.00f}, // borderShadow

  // ======== Frame ========

  {0.135f, 0.140f, 0.155f, 1.0f}, // frameBg
  {0.185f, 0.190f, 0.210f, 1.0f}, // frameBgHovered
  {0.215f, 0.220f, 0.240f, 1.0f}, // frameBgActive

  // ======== Button ========

  {0.145f, 0.150f, 0.165f, 1.0f}, // button
  {0.215f, 0.220f, 0.240f, 1.0f}, // buttonHovered
  {0.265f, 0.270f, 0.295f, 1.0f}, // buttonActive

  // ======== Header ========

  {0.155f, 0.160f, 0.175f, 1.0f}, // header
  {0.215f, 0.220f, 0.240f, 1.0f}, // headerHovered
  {0.275f, 0.280f, 0.305f, 1.0f}, // headerActive

  // ======== Text ========

  {0.91f, 0.92f, 0.94f, 1.0f},  // text
  {0.52f, 0.54f, 0.58f, 1.0f},  // textDisabled
  {0.20f, 0.32f, 0.50f, 0.50f}, // textSelectedBg

  // ======== Check Mark ========

  {0.75f, 0.78f, 0.82f, 1.0f}, // checkMark

  // ======== Slider ========

  {0.38f, 0.40f, 0.44f, 1.0f}, // sliderGrab
  {0.52f, 0.54f, 0.58f, 1.0f}, // sliderGrabActive

  // ======== Scrollbar ========

  {0.055f, 0.060f, 0.070f, 1.0f}, // scrollbarBg
  {0.25f, 0.26f, 0.29f, 1.0f},    // scrollbarGrab
  {0.32f, 0.33f, 0.37f, 1.0f},    // scrollbarGrabHovered
  {0.40f, 0.41f, 0.45f, 1.0f},    // scrollbarGrabActive

  // ======== Separator ========

  {0.20f, 0.21f, 0.23f, 1.0f}, // separator
  {0.35f, 0.36f, 0.39f, 1.0f}, // separatorHovered
  {0.45f, 0.46f, 0.50f, 1.0f}, // separatorActive

  // ======== Resize Grip ========

  {0.20f, 0.21f, 0.23f, 0.30f}, // resizeGrip
  {0.35f, 0.36f, 0.39f, 0.60f}, // resizeGripHovered
  {0.45f, 0.46f, 0.50f, 0.80f}, // resizeGripActive

  // ======== Tab ========

  {0.125f, 0.130f, 0.145f, 1.0f}, // tab
  {0.215f, 0.220f, 0.240f, 1.0f}, // tabHovered
  {0.18f, 0.19f, 0.21f, 1.0f},    // tabSelected
  {0.30f, 0.55f, 0.85f, 1.0f},    // tabSelectedOverline
  {0.10f, 0.105f, 0.115f, 1.0f},  // tabDimmed
  {0.15f, 0.155f, 0.17f, 1.0f},   // tabDimmedSelected
  {0.24f, 0.40f, 0.65f, 1.0f},    // tabDimmedSelectedOverline

  // ======== Docking ========

  {0.30f, 0.55f, 0.85f, 0.70f},   // dockingPreview
  {0.075f, 0.080f, 0.090f, 1.0f}, // dockingEmptyBg

  // ======== Table ========

  {0.14f, 0.145f, 0.16f, 1.0f},   // tableHeaderBg
  {0.24f, 0.25f, 0.27f, 1.0f},    // tableBorderStrong
  {0.18f, 0.19f, 0.21f, 1.0f},    // tableBorderLight
  {0.10f, 0.105f, 0.115f, 1.0f},  // tableRowBg
  {0.125f, 0.130f, 0.145f, 1.0f}, // tableRowBgAlt

  // ======== Plot ========

  {0.45f, 0.65f, 0.90f, 1.0f}, // plotLines
  {0.65f, 0.80f, 1.00f, 1.0f}, // plotLinesHovered
  {0.40f, 0.65f, 0.90f, 1.0f}, // plotHistogram
  {0.60f, 0.80f, 1.00f, 1.0f}, // plotHistogramHovered

  // ======== Navigation ========

  {0.30f, 0.55f, 0.85f, 0.50f}, // navHighlight
  {0.35f, 0.60f, 0.90f, 0.60f}, // navWindowingHighlight
  {0.00f, 0.00f, 0.00f, 0.40f}, // navWindowingDimBg
  {0.00f, 0.00f, 0.00f, 0.55f}, // modalWindowDimBg

  // ======== Drag & Drop ========

  {0.35f, 0.65f, 1.00f, 1.0f}, // dragDropTarget

  // ======== Input ========

  {0.135f, 0.140f, 0.155f, 1.0f}, // inputBg
  {0.185f, 0.190f, 0.210f, 1.0f}, // inputHovered
  {0.215f, 0.220f, 0.240f, 1.0f}, // inputActive

  // ======== Tooltip ========

  {0.045f, 0.050f, 0.060f, 0.96f}, // tooltipBg

  // ======== Accent ========

  {0.30f, 0.55f, 0.85f, 1.0f}, // accent
  {0.40f, 0.63f, 0.92f, 1.0f}, // accentHovered
  {0.22f, 0.45f, 0.75f, 1.0f}, // accentActive

  // ======== Status ========

  {0.30f, 0.75f, 0.45f, 1.0f}, // success
  {0.95f, 0.70f, 0.25f, 1.0f}, // warning
  {0.90f, 0.30f, 0.30f, 1.0f}, // error
  {0.30f, 0.60f, 0.90f, 1.0f}, // info
};

static constexpr UIColorInfo lightUIColor{

  // ======== Window ========

  {0.94f, 0.945f, 0.955f, 1.0f}, // windowBg
  {0.90f, 0.905f, 0.915f, 1.0f}, // childBg
  {0.975f, 0.975f, 0.98f, 1.0f}, // popupBg

  // ======== Border ========

  {0.75f, 0.76f, 0.78f, 1.0f},  // border
  {0.00f, 0.00f, 0.00f, 0.00f}, // borderShadow

  // ======== Frame ========

  {0.86f, 0.865f, 0.88f, 1.0f}, // frameBg
  {0.79f, 0.80f, 0.82f, 1.0f},  // frameBgHovered
  {0.72f, 0.73f, 0.76f, 1.0f},  // frameBgActive

  // ======== Button ========

  {0.86f, 0.865f, 0.88f, 1.0f}, // button
  {0.78f, 0.79f, 0.82f, 1.0f},  // buttonHovered
  {0.69f, 0.70f, 0.74f, 1.0f},  // buttonActive

  // ======== Header ========

  {0.85f, 0.855f, 0.87f, 1.0f}, // header
  {0.76f, 0.77f, 0.80f, 1.0f},  // headerHovered
  {0.68f, 0.69f, 0.73f, 1.0f},  // headerActive

  // ======== Text ========

  {0.12f, 0.125f, 0.14f, 1.0f}, // text
  {0.48f, 0.49f, 0.52f, 1.0f},  // textDisabled
  {0.30f, 0.50f, 0.75f, 0.25f}, // textSelectedBg

  // ======== Check Mark ========

  {0.20f, 0.22f, 0.25f, 1.0f}, // checkMark

  // ======== Slider ========

  {0.48f, 0.50f, 0.54f, 1.0f}, // sliderGrab
  {0.35f, 0.38f, 0.42f, 1.0f}, // sliderGrabActive

  // ======== Scrollbar ========

  {0.82f, 0.825f, 0.84f, 1.0f}, // scrollbarBg
  {0.60f, 0.61f, 0.64f, 1.0f},  // scrollbarGrab
  {0.50f, 0.51f, 0.54f, 1.0f},  // scrollbarGrabHovered
  {0.40f, 0.41f, 0.44f, 1.0f},  // scrollbarGrabActive

  // ======== Separator ========

  {0.76f, 0.77f, 0.79f, 1.0f}, // separator
  {0.55f, 0.57f, 0.60f, 1.0f}, // separatorHovered
  {0.42f, 0.44f, 0.47f, 1.0f}, // separatorActive

  // ======== Resize Grip ========

  {0.55f, 0.56f, 0.59f, 0.30f}, // resizeGrip
  {0.40f, 0.41f, 0.44f, 0.60f}, // resizeGripHovered
  {0.30f, 0.32f, 0.35f, 0.80f}, // resizeGripActive

  // ======== Tab ========

  {0.88f, 0.885f, 0.90f, 1.0f},  // tab
  {0.78f, 0.79f, 0.82f, 1.0f},   // tabHovered
  {0.94f, 0.945f, 0.955f, 1.0f}, // tabSelected
  {0.30f, 0.55f, 0.85f, 1.0f},   // tabSelectedOverline
  {0.85f, 0.855f, 0.87f, 1.0f},  // tabDimmed
  {0.90f, 0.905f, 0.915f, 1.0f}, // tabDimmedSelected
  {0.35f, 0.55f, 0.80f, 1.0f},   // tabDimmedSelectedOverline

  // ======== Docking ========

  {0.30f, 0.55f, 0.85f, 0.45f},  // dockingPreview
  {0.90f, 0.905f, 0.915f, 1.0f}, // dockingEmptyBg

  // ======== Table ========

  {0.84f, 0.845f, 0.86f, 1.0f},  // tableHeaderBg
  {0.68f, 0.69f, 0.72f, 1.0f},   // tableBorderStrong
  {0.78f, 0.79f, 0.81f, 1.0f},   // tableBorderLight
  {0.94f, 0.945f, 0.955f, 1.0f}, // tableRowBg
  {0.90f, 0.905f, 0.915f, 1.0f}, // tableRowBgAlt

  // ======== Plot ========

  {0.20f, 0.45f, 0.75f, 1.0f}, // plotLines
  {0.15f, 0.35f, 0.65f, 1.0f}, // plotLinesHovered
  {0.20f, 0.45f, 0.75f, 1.0f}, // plotHistogram
  {0.15f, 0.35f, 0.65f, 1.0f}, // plotHistogramHovered

  // ======== Navigation ========

  {0.30f, 0.55f, 0.85f, 0.45f}, // navHighlight
  {0.30f, 0.55f, 0.85f, 0.50f}, // navWindowingHighlight
  {0.00f, 0.00f, 0.00f, 0.20f}, // navWindowingDimBg
  {0.00f, 0.00f, 0.00f, 0.32f}, // modalWindowDimBg

  // ======== Drag & Drop ========

  {0.25f, 0.55f, 0.90f, 1.0f}, // dragDropTarget

  // ======== Input ========

  {0.86f, 0.865f, 0.88f, 1.0f}, // inputBg
  {0.79f, 0.80f, 0.82f, 1.0f},  // inputHovered
  {0.72f, 0.73f, 0.76f, 1.0f},  // inputActive

  // ======== Tooltip ========

  {0.98f, 0.98f, 0.985f, 0.98f}, // tooltipBg

  // ======== Accent ========

  {0.25f, 0.50f, 0.80f, 1.0f}, // accent
  {0.35f, 0.60f, 0.90f, 1.0f}, // accentHovered
  {0.18f, 0.40f, 0.70f, 1.0f}, // accentActive

  // ======== Status ========

  {0.18f, 0.65f, 0.35f, 1.0f}, // success
  {0.85f, 0.55f, 0.10f, 1.0f}, // warning
  {0.80f, 0.20f, 0.20f, 1.0f}, // error
  {0.20f, 0.50f, 0.80f, 1.0f}, // info
};

class ColorMan {
public:
  void init(const UIColorInfo &info = darkUIColor);
  void set(PresetUIColor color);
  void set(PresetUIColor color, const UIColorInfo &info);
  void reset();
  void apply(); // 把配色写入 ImGuiStyle

  SV_FORCE_INLINE PresetUIColor color() const { return m_color; }
  SV_FORCE_INLINE const UIColorInfo &info() const { return m_info; }

private:
  PresetUIColor m_color;
  UIColorInfo m_info;
};
} // namespace UI
