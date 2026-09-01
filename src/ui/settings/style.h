#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"

namespace UI {
// ======= UI Style =======

enum class PresetUIStyle {
  rectangle,
  roundedRectangle,
  roundedHeadRectangle,
  custom
};

// Style info in settings, global style setting
struct StyleInfo {
  f32 windowRounding;
  f32 childRounding;
  f32 frameRounding;
  f32 popupRounding;
  f32 scrollbarRounding;
  f32 grabRounding;
  f32 tabRounding;
};

static constexpr StyleInfo rectangleUIStyle{

  // Window
  0.0f,

  // Child
  0.0f,

  // Frame
  0.0f,

  // Popup
  0.0f,

  // Scrollbar
  0.0f,

  // Grab
  0.0f,

  // Tab
  0.0f,
};

static constexpr StyleInfo roundedRectangleUIStyle{
  // Window
  6.0f,

  // Child
  5.0f,

  // Frame
  4.0f,

  // Popup
  6.0f,

  // Scrollbar
  4.0f,

  // Grab
  4.0f,

  // Tab
  4.0f,
};

static constexpr StyleInfo roundedHeadRectangleUIStyle{
  // Window
  2.0f,

  // Child
  2.0f,

  // Frame
  8.0f,

  // Popup
  3.0f,

  // Scrollbar
  8.0f,

  // Grab
  8.0f,

  // Tab
  8.0f,
};

class Style {
public:
  void init(const StyleInfo &info = roundedRectangleUIStyle);
  void set(PresetUIStyle preset);
  void set(PresetUIStyle preset, const StyleInfo &info);
  void reset();
  void apply(); // 圆角写入 ImGuiStyle

  SV_FORCE_INLINE PresetUIStyle preset() const { return m_preset; }
  SV_FORCE_INLINE const StyleInfo &info() const { return m_info; }

private:
  PresetUIStyle m_preset{PresetUIStyle::roundedRectangle};
  StyleInfo m_info{roundedRectangleUIStyle};
};

} // namespace UI
