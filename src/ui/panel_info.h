#pragma once

#include "core/define/types.h"

#include <imgui.h>
#include <optional>

namespace UI {
enum class PanelType { window, child, dockspace };
enum class PanelFlag : u32 {
  none = 0,
  movable = 1 << 0,
  resizable = 1 << 1,
  collapsible = 1 << 2,
  closable = 1 << 3,
  dockable = 1 << 4,
  scrollable = 1 << 5,
  transparent = 1 << 6, // 不画背景（Scene 视口用）

};

constexpr PanelFlag operator|(PanelFlag lhs, PanelFlag rhs) {
  return static_cast<PanelFlag>(static_cast<u32>(lhs) | static_cast<u32>(rhs));
}

constexpr bool hasFlag(PanelFlag flags, PanelFlag flag) {
  return (static_cast<u32>(flags) & static_cast<u32>(flag)) != 0;
}

// ======== Panel Style：每个面板自己的圆角配置 ========
struct PanelStyle {
  f32 windowRounding{6.f};
  f32 childRounding{5.f};
  f32 frameRounding{4.f};
  f32 popupRounding{6.f};
  f32 scrollbarRounding{4.f};
  f32 grabRounding{4.f};
  f32 tabRounding{4.f};
};

// 普通矩形（顶栏/底栏用）
constexpr PanelStyle rectanglePanelStyle{0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
// 圆角矩形（默认）
constexpr PanelStyle roundedPanelStyle{6.f, 5.f, 4.f, 6.f, 4.f, 4.f, 4.f};

// ======== Panel Info ========
struct PanelInfo {
  const char *title{"Demo"};
  const char *iconPath{nullptr};
  PanelType type{PanelType::window};
  PanelFlag flag{PanelFlag::movable | PanelFlag::resizable | PanelFlag::closable
                 | PanelFlag::dockable};
  bool open{true};
  ImVec2 defaultPos{0.f, 0.f};
  ImVec2 defaultSize{300.f, 300.f};
  // depends on Panel::resizable
  std::optional<ImVec2> minSize{};
  std::optional<ImVec2> maxSize{};
  PanelStyle style{roundedPanelStyle}; // 默认圆角；顶/底栏显式用矩形
};

} // namespace UI
