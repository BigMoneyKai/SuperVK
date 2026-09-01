#include "layout.h"
#include "imgui_internal.h"
#include <algorithm>

namespace UI {

namespace {
// 默认/边界值（splitter 位置没设置时用这些兜底）
constexpr f32 kDefaultSideBarWidth = 200.f;
constexpr f32 kDefaultConsoleHeight = 220.f;
constexpr f32 kMinRightWidth = 200.f; // scene/console 区最小宽度
constexpr f32 kMinSceneHeight = 150.f;
constexpr f32 kMinConsoleHeight = 80.f;
} // namespace

void Layout::compute(const LayoutConfig &config, ImVec2 winSize) {
  m_config = config;

  // 窗口最小尺寸保护
  if (winSize.x < m_config.minWidth)
    winSize.x = m_config.minWidth;
  if (winSize.y < m_config.minHeight)
    winSize.y = m_config.minHeight;

  // --- 静态状态：上次窗口尺寸、各面板权重 ---
  static ImVec2 s_lastWinSize(0.f, 0.f);
  static bool s_initialized = false;
  static float s_fileWeight = 0.5f;     // 文件区宽度占总剩余宽度的比例
  static float s_sceneWeight = 0.5f;    // 场景区宽度占比（与文件区互补）
  static float s_sceneHWeight = 0.5f;   // 场景区高度占总剩余高度的比例
  static float s_consoleHWeight = 0.5f; // 控制台高度占比

  const f32 spacing = m_config.panelSpacing;
  const f32 splitW = m_config.splitterWidth;
  const f32 sideBarW = m_config.sideBarWidth;

  // 检测窗口是否 resize
  bool resized = (winSize.x != s_lastWinSize.x || winSize.y != s_lastWinSize.y);

  if (resized && s_initialized) {
    // ---- 根据存储的权重重新计算 splitter 位置 ----
    // 水平可用宽度（不包含分割器）
    f32 totalW = winSize.x - sideBarW - 2.f * spacing - splitW;
    if (totalW < 0.f)
      totalW = 0.f;
    f32 fileW = totalW * s_fileWeight;
    f32 sceneW = totalW * s_sceneWeight;

    // 裁剪：保证文件区和场景区的最小宽度
    f32 minSplitter1X =
      sideBarW + 2.f * spacing + m_config.defaultFileZoneWidth;
    f32 maxSplitter1X = winSize.x - splitW - kMinRightWidth;
    f32 splitter1X = sideBarW + 2.f * spacing + fileW;
    if (splitter1X < minSplitter1X)
      splitter1X = minSplitter1X;
    if (splitter1X > maxSplitter1X)
      splitter1X = maxSplitter1X;
    m_config.splitter1Pos.x = splitter1X;

    // 垂直可用高度（不包含分割器）
    f32 totalH = (winSize.y - m_config.topBarHeight - m_config.bottomBarHeight
                  - 2.f * spacing)
                 - splitW;
    if (totalH < 0.f)
      totalH = 0.f;
    f32 sceneH = totalH * s_sceneHWeight;
    f32 consoleH = totalH * s_consoleHWeight;

    // 裁剪：保证场景和控制台的最小高度
    f32 minSplitter2Y = m_config.topBarHeight + spacing + kMinSceneHeight;
    f32 maxSplitter2Y = winSize.y - m_config.bottomBarHeight - spacing
                        - kMinConsoleHeight - splitW;
    f32 splitter2Y = m_config.topBarHeight + spacing + sceneH;
    if (splitter2Y < minSplitter2Y)
      splitter2Y = minSplitter2Y;
    if (splitter2Y > maxSplitter2Y)
      splitter2Y = maxSplitter2Y;
    m_config.splitter2Pos.y = splitter2Y;
  }

  // ---- 原有矩形计算（保留裁剪保护，保证最终位置合法） ----
  // 1) 顶栏和底栏
  m_topBarRect = ImRect(0, 0, winSize.x, m_config.topBarHeight);
  f32 bottomBarTop = winSize.y - m_config.bottomBarHeight;
  m_bottomBarRect = ImRect(0, bottomBarTop, winSize.x, winSize.y);

  // 2) 侧栏
  f32 sideBarTop = m_config.topBarHeight + spacing;
  f32 sideBarBottom = bottomBarTop - spacing;
  f32 sideBarRight = m_config.sideBarWidth + spacing;
  m_sideBarRect = ImRect(0, sideBarTop, sideBarRight, sideBarBottom);

  // 3) splitter1 保护（若 resize 分支未触发，此处仍会限制用户拖拽的极端值）
  f32 minSplitter1X = sideBarRight + m_config.defaultFileZoneWidth;
  f32 maxSplitter1X = winSize.x * 0.5f; // 保证右侧至少 kMinRightWidth
  if (m_config.splitter1Pos.x < minSplitter1X)
    m_config.splitter1Pos.x = minSplitter1X;
  if (m_config.splitter1Pos.x > maxSplitter1X)
    m_config.splitter1Pos.x = maxSplitter1X;

  f32 splitter1Left = m_config.splitter1Pos.x;
  f32 splitter1Right = splitter1Left + splitW;
  m_splitter1Rect =
    ImRect(splitter1Left, sideBarTop, splitter1Right, sideBarBottom);

  // 4) 文件区
  f32 fileZoneLeft = sideBarRight + spacing;
  f32 fileZoneRight = splitter1Left;
  m_fileZoneRect =
    ImRect(fileZoneLeft, sideBarTop, fileZoneRight, sideBarBottom);

  // 5) splitter2 保护
  f32 minSplitter2Y = m_config.topBarHeight + kMinSceneHeight;
  f32 maxSplitter2Y = bottomBarTop - kMinConsoleHeight - splitW;
  if (m_config.splitter2Pos.y < minSplitter2Y)
    m_config.splitter2Pos.y = minSplitter2Y;
  if (m_config.splitter2Pos.y > maxSplitter2Y)
    m_config.splitter2Pos.y = maxSplitter2Y;

  f32 splitter2Top = m_config.splitter2Pos.y;
  f32 splitter2Bottom = splitter2Top + splitW;
  m_splitter2Rect =
    ImRect(splitter1Right, splitter2Top, winSize.x, splitter2Bottom);

  // 6) 场景区
  m_sceneRect = ImRect(splitter1Right, sideBarTop, winSize.x, splitter2Top);

  // 7) 控制台区
  m_consoleRect =
    ImRect(splitter1Right, splitter2Bottom, winSize.x, bottomBarTop - spacing);

  // ---- 从最终矩形推导权重，用于下次 resize ----
  const f32 fileZoneW = m_fileZoneRect.GetWidth();
  const f32 sceneW = m_sceneRect.GetWidth();
  const f32 sceneH = m_sceneRect.GetHeight();
  const f32 consoleH = m_consoleRect.GetHeight();

  const f32 totalW = fileZoneW + sceneW;
  const f32 totalH = sceneH + consoleH;

  if (totalW > 0.f) {
    s_fileWeight = fileZoneW / totalW;
    s_sceneWeight = sceneW / totalW;
  }
  if (totalH > 0.f) {
    s_sceneHWeight = sceneH / totalH;
    s_consoleHWeight = consoleH / totalH;
  }

  // 记录本次窗口尺寸，标记已初始化
  s_lastWinSize = winSize;
  s_initialized = true;

  // 推导宽度权重（保留原有逻辑，但不再使用，仅供参考）
  m_fileZoneWidthWeight = totalW > 0.f ? fileZoneW / totalW : 0.f;
  m_sceneWidthWeight = totalW > 0.f ? sceneW / totalW : 0.f;
  m_consoleWidthWeight = m_sceneWidthWeight;
  m_sceneHeightWeight = totalH > 0.f ? sceneH / totalH : 0.f;
  m_consoleHeightWeight = totalH > 0.f ? consoleH / totalH : 0.f;
}

void Layout::reset() {
  m_config = LayoutConfig{}; // 恢复默认 splitter 位置，下帧 compute 重新生效
}

} // namespace UI
