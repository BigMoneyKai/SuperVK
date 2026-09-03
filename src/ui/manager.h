#pragma once

#include "core/define/types.h"
#include "ui/panel/bottom_bar.h"
#include "ui/panel/console.h"
#include "ui/panel/file_zone.h"
#include "ui/panel/scene.h"
#include "ui/panel/settings_panel.h"
#include "ui/panel/side_bar.h"
#include "ui/panel/splitter.h"
#include "ui/panel/stats.h"
#include "ui/panel/top_bar.h"
#include "ui/layout.h"
#include "ui/settings.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace UI {
struct UIInitInfo {
  GLFWwindow *window;
  VkInstance instance;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  u32 queueFamily;
  VkQueue queue;
  VkRenderPass renderPass;
  u32 imageCount;
};

class Man {
public:
  void init(const UIInitInfo &info);
  void newFrame();
  void endFrame();
  void update(VkCommandBuffer cmd); // 渲染 hook 内调用（render pass 中）
  void destroy();
  b32 captureMouseNeeded();

  // Scene 视口矩形（ImGui 窗口坐标），App 用来换算成 framebuffer 像素
  SV_FORCE_INLINE const ImRect &viewportRect() const {
    return m_layout.sceneRect();
  }

private:
  GLFWwindow *m_window;
  VkInstance m_instance;
  VkPhysicalDevice m_physicalDevice;
  VkDevice m_device;
  u32 m_queueFamily;
  VkQueue m_queue;
  VkRenderPass m_renderPass;
  u32 m_imageCount;

  BottomBar m_bottomBar;
  Console m_console;
  FileZone m_fileZone;
  Scene m_scene;
  SideBar m_sideBar;
  Stats m_stats;
  TopBar m_topBar;
  Splitter m_splitter1; // Between file zone and scene
  Splitter m_splitter2; // Between console and scene
  SettingsPanel m_settingsPanel;

  Layout m_layout;
  Settings m_settings;
};
} // namespace UI
