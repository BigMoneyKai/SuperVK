#include "window/window_manager.h"
#include "core/debug/debugger.h"
#include "core/debug/logger_attrib.h"
#include "utils/utils.h"

WinMan::WinMan() {
  m_window = nullptr;
  m_width = 0;
  m_height = 0;
  m_headless = SV_FALSE;

  m_monitor = nullptr;
}

void WinMan::init(const char *title, DisplayMode displayMode) {
  if (!glfwInit()) {
    FATAL(LogCatag::Window, "Failed to initialize GLFW");
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#if defined(__linux__)
  // TODO: Linux specific platform settings
#elif defined(__APPLE__)
  // glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#elif defined(_WIN32)
  // TODO: Windows specific platform settings
#endif
  m_monitor = glfwGetPrimaryMonitor();
  m_mode = glfwGetVideoMode(m_monitor);
  if ((!m_monitor || !m_mode) && displayMode != WINDOWED) {
    FATAL(LogCatag::Window, "Failed to get monitor or mode");
  }

  switch (displayMode) {
  case FULLSCREEN:
    m_width = m_mode->width;
    m_height = m_mode->height;
    m_window =
        glfwCreateWindow(static_cast<i32>(m_width), static_cast<i32>(m_height),
                         title, m_monitor, nullptr);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
  case BORDERLESS:
    m_width = m_mode->width;
    m_height = m_mode->height;
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window =
        glfwCreateWindow(static_cast<i32>(m_width), static_cast<i32>(m_height),
                         title, nullptr, nullptr);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
  case WINDOWED:
    m_width = default_width;
    m_height = default_height;
    m_window =
        glfwCreateWindow(static_cast<i32>(m_width), static_cast<i32>(m_height),
                         title, nullptr, nullptr);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
  default:
    FATAL(LogCatag::Window, "Failed to load window");
  }
  if (!m_window) {
    FATAL(LogCatag::Window, "Failed to create window");
  }
}

void WinMan::destroy() {
  if (m_window) {
    glfwDestroyWindow(m_window);
    m_window = nullptr;
  }
  glfwTerminate();
}
