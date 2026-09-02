#pragma once

#include "base/command_buffer.h"
#include "base/command_pool.h"
#include "base/device.h"
#include "base/instance.h"
#include "base/surface.h"
#include "base/swapchain.h"
#include "descriptor/descriptor_manager.h"
#include "frame/depth_resource.h"
#include "frame/frame_resource.h"
#include "imgui_internal.h"
#include "pipeline/pipeline_manager.h"
#include "render/graph/render_graph.h"
#include "resource/buffer_manager.h"
#include "resource/texture_manager.h"
#include "window/window_manager.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

// Forward declaration
namespace Scene {
class Scene;
}

struct RendererDesc {
  Window::WinMan *pWinMan{nullptr};
  VkExtent2D swapchainExtent{};
};

class Renderer {
public:
  void init(const RendererDesc &desc);
  void render(Scene::Scene &scene);
  void waitIdle();
  void destroy();

  SV_FORCE_INLINE const Device &device() const { return m_device; }
  SV_FORCE_INLINE Device &device() { return m_device; }
  SV_FORCE_INLINE VkInstance instance() { return m_instance.instance(); }
  SV_FORCE_INLINE VkRenderPass renderPass() {
    return m_renderGraph.renderPassOf(0);
  }
  SV_FORCE_INLINE u32 imageCount() { return m_swapchain.imageCount(); }
  SV_FORCE_INLINE VkExtent2D swapchainExtent() const {
    return m_swapchain.extent();
  }

  // 3D 场景只画在视口矩形里（framebuffer 像素坐标，由 UI 布局提供）
  void setViewportRect(i32 x1, i32 y1, i32 x2, i32 y2) {
    if (x1 >= x2 || y1 >= y2) { // 防御：非法矩形回退全屏，避免 Vulkan 校验报错
      m_hasViewport = SV_FALSE;
      return;
    }
    m_viewportRect = ImRect(x1, y1, x2, y2);
    m_hasViewport = SV_TRUE;
  }

public:
  using UIRenderFn = void (*)(VkCommandBuffer, void *userData);
  void setUiRenderHook(UIRenderFn fn, void *userData) {
    m_uiRenderFn = fn;
    m_uiUserData = userData;
  }

private:
  void drawFrame(Scene::Scene &scene);
  void rebuildSwapchain(u32 width, u32 height);

private:  
  // ------ target class members ------
  Instance m_instance;
  Device m_device;
  Surface m_surface;
  Swapchain m_swapchain;

  CommandPool m_commandPool;
  CommandBuffer m_commandBuffer;

  PipelineMan m_pipelineMan;
  DescriptorMan m_descriptorMan;
  BufferMan m_bufferMan;
  TextureMan m_textureMan;

  FrameResource m_frameResource;
  DepthResource m_depthResource;
  RenderGraph m_renderGraph;
  RenderGraphResource m_presenthRGR{};
  RenderGraphResource m_depthRGR{};

  RendererDesc m_desc;
  Scene::Scene *m_scene{nullptr};
  u32 m_currFrame{0};
  u32 m_imageIndex{0};

  ImRect m_viewportRect{};
  b32 m_hasViewport{SV_FALSE};

  UIRenderFn m_uiRenderFn{nullptr};
  void *m_uiUserData{nullptr};
};
