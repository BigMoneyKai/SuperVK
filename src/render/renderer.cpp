#include "render/renderer.h"
#include "core/debug/debugger.h"
#include "scene/scene.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

void Renderer::init(const RendererDesc &desc) {
  m_desc = desc;
  m_instance.init();
  m_surface.init(m_instance.instance(), desc.pWinMan->window());
  m_device.init(m_instance.instance(), m_surface.surface());

  // 初始 swapchain 用实际 framebuffer 像素尺寸（不能用窗口 points）
  i32 initW = 0, initH = 0;
  glfwGetFramebufferSize(desc.pWinMan->window(), &initW, &initH);
  m_swapchain.init(
    m_device.device(), m_device.physicalDevice(), m_surface.surface(),
    {initW > 0 ? static_cast<u32>(initW) : desc.pWinMan->width(),
     initH > 0 ? static_cast<u32>(initH) : desc.pWinMan->height()});

  m_renderGraph.init(m_device.device(), m_swapchain.extent());
  m_renderGraph.setPresentTarget(m_swapchain.swapchainImages()[0],
                                 m_swapchain.swapchainImageViews()[0],
                                 m_swapchain.colorFormat());

  m_descriptorMan.init(m_device.device(), m_device.physicalDevice());

  m_frameResource.init(m_device.device(), m_swapchain.imageCount());
  m_frameResource.createSyncPrimitives();
  m_depthResource.init(m_device.device(), m_device.physicalDevice(),
                       m_device.depthFormat(), m_swapchain.extent());

  m_depthRGR = m_renderGraph.registerExternal(
    m_depthResource.depthImage(), m_depthResource.depthImageView(),
    m_device.depthFormat(), VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

  m_renderGraph.addPass(
    "Main",
    {.colorAttachments = {{m_renderGraph.presentTarget(), LoadOp::clear,
                           StoreOp::store}},
     .depthAttachment = {m_depthRGR.id, LoadOp::clear, StoreOp::store},
     .drawList = [this](VkCommandBuffer cmd) {
       if (m_scene == nullptr)
         return;
       Scene::Scene *scene = m_scene;

       // ---- update UBOs from scene data ----
       m_descriptorMan.updateCameraUBO(m_currFrame, &scene->camera().ubo(),
                                       sizeof(CameraUBO));
       m_descriptorMan.updateObjectUBO(m_currFrame, &scene->object().ubo(),
                                       sizeof(ObjectUBO));
       m_descriptorMan.updateLightUBO(m_currFrame, &scene->light().ubo(),
                                      sizeof(LightUBO));
       m_descriptorMan.updateMaterialUBO(m_currFrame, &scene->material().ubo(),
                                         sizeof(MaterialUBO));

       // ---- bind pipeline ----
       vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                         m_pipelineMan.graphicsPipeline());

       // ---- bind descriptor set ----
       VkDescriptorSet ds = m_descriptorMan.descriptorSet(m_currFrame);
       vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                               m_pipelineMan.graphicsPipelineLayout(), 0, 1,
                               &ds, 0, nullptr);

       // ---- dynamic viewport + scissor ----
       VkViewport viewport{};
       VkRect2D scissor{};
       if (m_hasViewport) {
         viewport.x = static_cast<f32>(m_viewportRect.Min.x);
         viewport.y = static_cast<f32>(m_viewportRect.Min.y);
         viewport.width = m_viewportRect.GetWidth();
         viewport.height = m_viewportRect.GetHeight();
         scissor.offset = {static_cast<i32>(m_viewportRect.Min.x),
                           static_cast<i32>(m_viewportRect.Min.y)};
         scissor.extent = {static_cast<u32>(m_viewportRect.GetWidth()),
                           static_cast<u32>(m_viewportRect.GetHeight())};
       } else {
         viewport.width = static_cast<f32>(m_swapchain.extent().width);
         viewport.height = static_cast<f32>(m_swapchain.extent().height);
         scissor.offset = {0, 0};
         scissor.extent = m_swapchain.extent();
       }
       viewport.minDepth = 0.0f;
       viewport.maxDepth = 1.0f;
       vkCmdSetViewport(cmd, 0, 1, &viewport);

       vkCmdSetScissor(cmd, 0, 1, &scissor);

       // ---- bind vertex + index buffers ----
       VkBuffer vb = scene->mesh().vertexBuffer().buffer();
       VkDeviceSize vbOffsets[1] = {0};
       vkCmdBindVertexBuffers(cmd, 0, 1, &vb, vbOffsets);
       vkCmdBindIndexBuffer(cmd, scene->mesh().indexBuffer().buffer(), 0,
                            VK_INDEX_TYPE_UINT32);

       // ---- draw ----
       u32 indexCount = static_cast<u32>(scene->mesh().indices().size());
       vkCmdDrawIndexed(cmd, indexCount, 1, 0, 0, 0);

       // ---- debug UI overlay (ImGui) ----
       if (m_uiRenderFn)
         m_uiRenderFn(cmd, m_uiUserData);
     }});

  // 先生成 render pass，管线创建时需要它
  m_renderGraph.process();

  m_pipelineMan.init(m_device.device(), m_renderGraph.renderPassOf(0),
                     m_descriptorMan.layout());
  m_bufferMan.init();

  m_commandPool.init(m_device.device(), m_device.graphicsQueueFamilyIndex());
  m_commandBuffer.init(m_device.device(), m_commandPool.pool(),
                       m_frameResource.framebufferCount());

  m_textureMan.init(m_device.device(), m_device.physicalDevice(),
                    m_commandPool.pool(), m_device.graphicsQueue());
  m_descriptorMan.writeBufferDescriptorSet();

  u32 texIndex = m_textureMan.loadTexture("resource/textures/mamba.png");
  m_descriptorMan.writeImageDescriptorSet(
    m_currFrame, m_textureMan.texture(texIndex).imageView(),
    m_textureMan.texture(texIndex).imageLayout());

  DEBUG(LogCatag::render, "Renderer initialized");
}

void Renderer::render(Scene::Scene &scene) {
  m_scene = &scene;

  drawFrame(scene);
}

void Renderer::waitIdle() { vkDeviceWaitIdle(m_device.device()); }

void Renderer::destroy() {
  // 渲染图（render pass / framebuffer）
  m_renderGraph.destroy();

  // sync, depth, commands
  m_frameResource.destroy();
  m_depthResource.destroy();

  m_textureMan.destroy();

  m_commandPool.destroy();

  // pipeline + descriptors
  m_pipelineMan.destroy();
  m_descriptorMan.destroy();

  // swapchain, surface, device, instance
  m_swapchain.destroy();
  m_surface.destroy();
  m_device.destroy();
  m_instance.destroy();
}

void Renderer::drawFrame(Scene::Scene &scene) {
  VkFence fence = m_frameResource.inFlightFence(m_currFrame);
  vkWaitForFences(m_device.device(), 1, &fence, VK_TRUE, UINT64_MAX);
  vkResetFences(m_device.device(), 1, &fence);
  VkSemaphore imageSem = m_frameResource.imageAvailableSemaphore(m_currFrame);

  VkResult result =
    vkAcquireNextImageKHR(m_device.device(), m_swapchain.swapchain(),
                          UINT64_MAX, imageSem, VK_NULL_HANDLE, &m_imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    i32 w, h;
    glfwGetFramebufferSize(m_desc.pWinMan->window(), &w, &h);
    if (w > 0 && h > 0) {
      rebuildSwapchain(w, h);
    }
    return; // 不推进帧索引，下一帧重试
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    FATAL(LogCatag::render, "Failed to acquire swapchain image");
  }

  // ----- 3. 录制 Command Buffer -----
  VkCommandBuffer cmdBuf = m_commandBuffer.get(m_currFrame);
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  // 使用 VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT 可优化
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  if (vkBeginCommandBuffer(cmdBuf, &beginInfo) != VK_SUCCESS) {
    FATAL(LogCatag::render, "Failed to begin command buffer");
  }

  // 更新渲染目标的交换链图像
  m_renderGraph.setPresentTarget(
    m_swapchain.swapchainImages()[m_imageIndex],
    m_swapchain.swapchainImageViews()[m_imageIndex], m_swapchain.colorFormat());

  // 重新生成 RenderGraph（内部会更新 Framebuffer 尺寸）
  m_renderGraph.process();
  m_renderGraph.execute(cmdBuf);

  // ----- 4. （重要）更新 ImGui 尺寸（防止 UI 拉伸） -----
  // 如果 m_uiRenderFn 被调用，确保它在录制前获取最新尺寸
  // 但最好在外部（如主循环）每帧更新 io.DisplaySize
  // 这里可以在执行 m_renderGraph 之前就设置好，但需要访问 ImGui。
  // 推荐在 m_uiRenderFn 内部更新，或在这里设置：
  int win_w, win_h, fb_w, fb_h;
  glfwGetWindowSize(m_desc.pWinMan->window(), &win_w, &win_h);
  glfwGetFramebufferSize(m_desc.pWinMan->window(), &fb_w, &fb_h);
  ImGui::GetIO().DisplaySize = ImVec2((float)win_w, (float)win_h);
  ImGui::GetIO().DisplayFramebufferScale =
    ImVec2((float)fb_w / win_w, (float)fb_h / win_h);

  if (vkEndCommandBuffer(cmdBuf) != VK_SUCCESS) {
    FATAL(LogCatag::render, "Failed to end command buffer");
  }

  // ----- 5. 提交渲染 -----
  VkSemaphore finishSem = m_frameResource.renderFinishedSemaphore(m_currFrame);
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &cmdBuf;

  VkPipelineStageFlags waitStage =
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = &imageSem;
  submitInfo.pWaitDstStageMask = &waitStage;

  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = &finishSem;

  if (vkQueueSubmit(m_device.graphicsQueue(), 1, &submitInfo, fence)
      != VK_SUCCESS) {
    FATAL(LogCatag::render, "Failed to submit command buffer");
  }

  // ----- 6. 呈现（处理可能的尺寸变化） -----
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &finishSem;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &m_swapchain.swapchain();
  presentInfo.pImageIndices = &m_imageIndex;

  result = vkQueuePresentKHR(m_device.presentQueue(), &presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    i32 w, h;
    glfwGetFramebufferSize(m_desc.pWinMan->window(), &w, &h);
    if (w > 0 && h > 0) {
      rebuildSwapchain(w, h);
    }
    // 注意：这里重建后不 return，但我们已经提交了当前帧的渲染，
    // 如果 present 失败，画面可能闪一下，但下一帧会使用新交换链。
    // 你也可以选择在 present 失败时不推进 m_currFrame，但为了简单，通常就推进。
  }

  // ----- 7. 推进帧索引 -----
  m_currFrame = (m_currFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::rebuildSwapchain(u32 width, u32 height) {
  if (width == 0 || height == 0)
    return;

  // Wait for idle process
  vkDeviceWaitIdle(m_device.device());

  // Destroy old resources
  m_renderGraph.destroyFramebuffer();
  m_frameResource.destroy();
  m_depthResource.destroy();
  m_swapchain.destroy();

  // Rebuild
  m_swapchain.init(m_device.device(), m_device.physicalDevice(),
                   m_surface.surface(), {width, height});
  m_depthResource.init(m_device.device(), m_device.physicalDevice(),
                       m_device.depthFormat(), m_swapchain.extent());
  m_frameResource.init(m_device.device(), m_swapchain.imageCount());
  m_frameResource.createSyncPrimitives();
  m_renderGraph.setExtent(m_swapchain.extent());

  m_renderGraph.updateExternalResource(
    m_depthRGR.id, m_depthResource.depthImage(),
    m_depthResource.depthImageView(), m_device.depthFormat());
}
