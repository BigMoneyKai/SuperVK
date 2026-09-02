#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"
#include <GLFW/glfw3.h>

namespace Window {
constexpr u32 defaultWidth = 1920;
constexpr u32 defaultHeight = 1080;

constexpr u32 defaultMinWidth = 640;
constexpr u32 defaultMinHeight = 480;
constexpr u32 defaultMaxWidth = UINT_MAX;
constexpr u32 defaultMaxHeight = UINT_MAX;

enum class DisplayMode {
  fullscreen,
  borderless,
  windowed,
};

class WinMan {
public:
  WinMan();
  void init(const char *title, DisplayMode display);
  void destroy();

  SV_FORCE_INLINE const GLFWwindow *window() const { return m_window; }
  SV_FORCE_INLINE GLFWwindow *window() { return m_window; }

  SV_FORCE_INLINE const u32 &width() const { return m_width; }
  SV_FORCE_INLINE u32 &width() { return m_width; }

  SV_FORCE_INLINE const u32 &height() const { return m_height; }
  SV_FORCE_INLINE u32 &height() { return m_height; }

  SV_FORCE_INLINE const b32 &headless() const { return m_headless; }
  SV_FORCE_INLINE b32 &headless() { return m_headless; }

private:
  GLFWwindow *m_window;
  u32 m_width;
  u32 m_height;
  b32 m_headless;

  const GLFWvidmode *m_mode;
  GLFWmonitor *m_monitor;
};
} // namespace Window

using DisplayMode = Window::DisplayMode;

