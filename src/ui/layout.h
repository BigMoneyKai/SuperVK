#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"

#include <cfloat>
#include <imgui.h>
#include <imgui_internal.h>

namespace UI {

// ============================================================
// Layout
// ============================================================

struct LayoutConfig {
  // Weight for each variant panel

  // Height
  static constexpr f32 topBarHeight = 16.f;
  static constexpr f32 bottomBarHeight = 16.f;
  static constexpr f32 defaultSceneHeight = 600.f;

  // Width
  static constexpr f32 sideBarWidth = 64.f;
  static constexpr f32 splitterWidth = 1.f;
  static constexpr f32 defaultFileZoneWidth = 200.f;

  // Panel spacing
  static constexpr f32 panelSpacing = 2.f;

  // max & min
  static constexpr f32 minWidth = 540.f;
  static constexpr f32 maxWidth = FLT_MAX;
  static constexpr f32 minHeight = 300.f;
  static constexpr f32 maxHeight = FLT_MAX;

  // The only variable - Splitter Position
  ImVec2 splitter1Pos = {
    sideBarWidth + panelSpacing + defaultFileZoneWidth,
    topBarHeight + panelSpacing}; // Between side bar and scene/console
  ImVec2 splitter2Pos = {sideBarWidth + panelSpacing + defaultFileZoneWidth
                           + panelSpacing * 0.5f,
                         topBarHeight + panelSpacing
                           + defaultSceneHeight}; // Between scene and console
};

class Layout {
public:
  void compute(const LayoutConfig &config, ImVec2 winSize);
  void reset();

  SV_FORCE_INLINE const ImRect &topBarRect() const { return m_topBarRect; }
  SV_FORCE_INLINE const ImRect &sideBarRect() const { return m_sideBarRect; }
  SV_FORCE_INLINE const ImRect &statsRect() const { return m_statsRect; }
  SV_FORCE_INLINE const ImRect &fileZoneRect() const { return m_fileZoneRect; }
  SV_FORCE_INLINE const ImRect &sceneRect() const { return m_sceneRect; }
  SV_FORCE_INLINE const ImRect &consoleRect() const { return m_consoleRect; }
  SV_FORCE_INLINE const ImRect &bottomBarRect() const {
    return m_bottomBarRect;
  }
  SV_FORCE_INLINE const ImRect &splitter1Rect() const {
    return m_splitter1Rect;
  }
  SV_FORCE_INLINE const ImRect &splitter2Rect() const {
    return m_splitter2Rect;
  }
  SV_FORCE_INLINE const ImRect &settingsRect() const { return m_settingsRect; }

  SV_FORCE_INLINE LayoutConfig &config() { return m_config; }
  SV_FORCE_INLINE const LayoutConfig &config() const { return m_config; }

  SV_FORCE_INLINE f32 sceneWidthWeight() const { return m_sceneWidthWeight; }
  SV_FORCE_INLINE f32 sceneHeightWeight() const { return m_sceneHeightWeight; }
  SV_FORCE_INLINE f32 consoleWidthWeight() const {
    return m_consoleWidthWeight;
  }
  SV_FORCE_INLINE f32 consoleHeightWeight() const {
    return m_consoleHeightWeight;
  }
  SV_FORCE_INLINE f32 fileZoneWidthWeight() const {
    return m_fileZoneWidthWeight;
  }

private:
  LayoutConfig m_config;

  ImRect m_bottomBarRect;
  ImRect m_consoleRect;
  ImRect m_fileZoneRect;
  ImRect m_sceneRect;
  ImRect m_sideBarRect;
  ImRect m_statsRect;
  ImRect m_topBarRect;

  ImRect m_splitter1Rect; // between file zone and scene
  ImRect m_splitter2Rect; // betweem console and scene

  ImRect m_settingsRect;

  // Weight
  f32 m_sceneWidthWeight;
  f32 m_sceneHeightWeight;
  f32 m_consoleWidthWeight;
  f32 m_consoleHeightWeight;
  f32 m_fileZoneWidthWeight;
};
} // namespace UI
