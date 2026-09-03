#include "ui/manager.h"
#include "core/debug/debugger.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

void UI::Man::init(const UIInitInfo &info) {
  m_window = info.window;
  m_instance = info.instance;
  m_physicalDevice = info.physicalDevice;
  m_device = info.device;
  m_queueFamily = info.queueFamily;
  m_queue = info.queue;
  m_renderPass = info.renderPass;
  m_imageCount = info.imageCount;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForVulkan(m_window, SV_TRUE);

  ImGui_ImplVulkan_InitInfo vkInfo{};
  vkInfo.ApiVersion = VK_API_VERSION_1_0; // 与 Instance 创建的版本一致
  vkInfo.Instance = m_instance;
  vkInfo.PhysicalDevice = m_physicalDevice;
  vkInfo.Device = m_device;
  vkInfo.QueueFamily = m_queueFamily;
  vkInfo.Queue = m_queue;
  vkInfo.DescriptorPoolSize = 8;
  vkInfo.MinImageCount = 2;
  vkInfo.ImageCount = m_imageCount;
  vkInfo.PipelineInfoMain.RenderPass = m_renderPass;
  vkInfo.CheckVkResultFn = [](VkResult err) {
    if (err != VK_SUCCESS)
      WARNING(LogCatag::render, "ImGui Vulkan result: {}",
              static_cast<i32>(err));
  };
  ImGui_ImplVulkan_Init(&vkInfo);

  // ---- 主题（字体/颜色/圆角由 Settings 统一管理）----
  m_settings.init();
  m_settings.apply();

  // ---- 面板初始化 ----
  m_topBar.init(PanelInfo{.title = "TopBar", .style = rectanglePanelStyle});
  m_sideBar.init(PanelInfo{.title = "SideBar"});
  m_stats.init(PanelInfo{.title = "Stats"});
  m_fileZone.init(PanelInfo{.title = "FileZone"});
  m_scene.init(PanelInfo{.title = "Scene", .flag = PanelFlag::transparent});
  m_console.init(ConsoleInfo{});
  m_bottomBar.init(
    PanelInfo{.title = "BottomBar", .style = rectanglePanelStyle});
  m_splitter1.bind(&m_layout, 1);
  m_splitter2.bind(&m_layout, 2);
  m_settingsPanel.init(
    PanelInfo{.title = "Settings", .flag = PanelFlag::scrollable});
  m_settingsPanel.bind(&m_settings);
}

// 面板内容必须在 ImGui::NewFrame() 与 ImGui::Render() 之间提交。
void UI::Man::newFrame() {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();

  // 尺寸同步：后端可能滞后/Retina 不准，这里手动对齐（必须在 NewFrame 之前）
  ImGuiIO &io = ImGui::GetIO();
  i32 winW = 0, winH = 0, fbW = 0, fbH = 0;
  glfwGetWindowSize(m_window, &winW, &winH);
  glfwGetFramebufferSize(m_window, &fbW, &fbH);
  if (winW > 0 && winH > 0) {
    io.DisplaySize = ImVec2(static_cast<f32>(winW), static_cast<f32>(winH));
    io.DisplayFramebufferScale =
        ImVec2(static_cast<f32>(fbW) / winW, static_cast<f32>(fbH) / winH);
  }

  ImGui::NewFrame();

  // 布局：每帧按窗口尺寸重算
  m_layout.compute(m_layout.config(), ImGui::GetIO().DisplaySize);

  m_console.update();
  m_bottomBar.setViewportSize(m_layout.sceneRect().GetSize());
  m_console.draw(m_layout.consoleRect());
  m_fileZone.draw(m_layout.fileZoneRect());
  m_scene.draw(m_layout.sceneRect());
  m_sideBar.draw(m_layout.sideBarRect());

  // 顶栏/底栏：小号字体 + 紧凑内边距，16px 高度能放下
  ImGui::PushFont(m_settings.font().smallFont());
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 1.f));
  m_topBar.draw(m_layout.topBarRect());
  m_bottomBar.draw(m_layout.bottomBarRect());
  ImGui::PopStyleVar();
  ImGui::PopFont();

  m_splitter1.draw(m_layout.splitter1Rect());
  m_splitter2.draw(m_layout.splitter2Rect());
}

void UI::Man::endFrame() { ImGui::Render(); }

// 渲染 hook：只把 ImGui 生成的顶点画进 render pass，不提交任何 UI 代码。
void UI::Man::update(VkCommandBuffer cmd) {
  ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
}

// 必须在 render pass / device 销毁之前调用（ImGui pipeline 引用 render pass）。
void UI::Man::destroy() {
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

b32 UI::Man::captureMouseNeeded() {
  return ImGui::GetIO().WantCaptureMouse ? SV_TRUE : SV_FALSE;
}
