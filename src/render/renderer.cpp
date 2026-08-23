#include "render/renderer.h"
#include "core/debug/debugger.h"
#include "scene/scene.h"

void Renderer::init(const RendererDesc &desc) {
  m_desc = desc;
  m_instance.init();
  m_surface.init(m_instance.instance(), desc.pWinMan->window());
  m_device.init(m_instance.instance(), m_surface.surface());
  m_swapchain.init(m_device.device(), m_device.physicalDevice(),
                   m_surface.surface(),
                   {desc.pWinMan->width(), desc.pWinMan->height()});

  // 渲染图：声明唯一的 pass（交换链颜色 + 深度），绘制细节放进 drawList
  m_renderGraph.init(m_device.device(), m_swapchain.extent());
  m_renderGraph.setPresentTarget(m_swapchain.swapchainImages()[0],
                                 m_swapchain.swapchainImageViews()[0],
                                 m_swapchain.colorFormat());

  // DescriptorMan must init BEFORE PipelineMan (pipeline needs the layout)
  m_descriptorMan.init(m_device.device(), m_device.physicalDevice());

  m_frameResource.init(m_device.device(), m_swapchain.imageCount());
  m_frameResource.createSyncPrimitives();
  m_depthResource.init(m_device.device(), m_device.physicalDevice(),
                       m_device.depthFormat(), m_swapchain.extent());

  auto depthRT = m_renderGraph.registerExternal(
      m_depthResource.depthImage(), m_depthResource.depthImageView(),
      m_device.depthFormat(), VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

  m_renderGraph.addPass(
      "Main",
      {.colorAttachments = {{m_renderGraph.presentTarget(), LoadOp::clear,
                             StoreOp::store}},
       .depthAttachment = {depthRT, LoadOp::clear, StoreOp::store},
       .drawList = [this](VkCommandBuffer cmd) {
         if (m_scene == nullptr)
           return;
         Scene *scene = m_scene;

         // ---- update UBOs from scene data ----
         m_descriptorMan.updateCameraUBO(m_currFrame, &scene->camera().ubo(),
                                         sizeof(CameraUBO));
         m_descriptorMan.updateObjectUBO(m_currFrame, &scene->object().ubo(),
                                         sizeof(ObjectUBO));
         m_descriptorMan.updateLightUBO(m_currFrame, &scene->light().ubo(),
                                        sizeof(LightUBO));
         m_descriptorMan.updateMaterialUBO(
             m_currFrame, &scene->material().ubo(), sizeof(MaterialUBO));

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
         viewport.x = 0.0f;
         viewport.y = 0.0f;
         viewport.width = static_cast<f32>(m_swapchain.extent().width);
         viewport.height = static_cast<f32>(m_swapchain.extent().height);
         viewport.minDepth = 0.0f;
         viewport.maxDepth = 1.0f;
         vkCmdSetViewport(cmd, 0, 1, &viewport);

         VkRect2D scissor{};
         scissor.offset = {0, 0};
         scissor.extent = m_swapchain.extent();
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

  u32 texIndex = m_textureMan.loadTexture("resource/textures/red.png");
  m_descriptorMan.writeImageDescriptorSet(
      m_currFrame, m_textureMan.texture(texIndex).imageView(),
      m_textureMan.texture(texIndex).imageLayout());

  DEBUG(LogCatag::render, "Renderer initialized");
}

void Renderer::render(Scene &scene) {
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

void Renderer::drawFrame(Scene &scene) {
  auto fence = m_frameResource.inFlightFence(m_currFrame);
  auto imageSem = m_frameResource.imageAvailableSemaphore(m_currFrame);

  // Wait for fence
  vkWaitForFences(m_device.device(), 1, &fence, VK_TRUE, UINT64_MAX);

  // Acquire swapchain image
  VkResult result = vkAcquireNextImageKHR(
      m_device.device(), m_swapchain.swapchain(), UINT64_MAX, imageSem,
      VK_NULL_HANDLE, &m_imageIndex);
  if (result != VK_SUCCESS) {
    WARNING(LogCatag::render, "Failed to acquire swapchain image");
    return;
  }
  // Reset fence
  vkResetFences(m_device.device(), 1, &fence);

  auto finishSem = m_frameResource.renderFinishedSemaphore(m_currFrame);

  // Record command buffer
  VkCommandBuffer cmdBuf = m_commandBuffer.get(m_currFrame);
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(cmdBuf, &beginInfo) != VK_SUCCESS) {
    FATAL(LogCatag::render, "Failed to record command buffer");
  }

  // 这一帧画到哪张交换链图上
  m_renderGraph.setPresentTarget(
      m_swapchain.swapchainImages()[m_imageIndex],
      m_swapchain.swapchainImageViews()[m_imageIndex],
      m_swapchain.colorFormat());

  m_renderGraph.process();
  m_renderGraph.execute(cmdBuf);

  if (vkEndCommandBuffer(cmdBuf) != VK_SUCCESS) {
    FATAL(LogCatag::render, "Failed to record command buffer");
  }

  // Submit command buffer
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

  if (vkQueueSubmit(m_device.graphicsQueue(), 1, &submitInfo, fence) !=
      VK_SUCCESS) {
    FATAL(LogCatag::render, "Failed to submit draw command buffer");
  }

  // Present
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &finishSem;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &m_swapchain.swapchain();
  presentInfo.pImageIndices = &m_imageIndex;

  VK_CHECK_RESULT(vkQueuePresentKHR(m_device.presentQueue(), &presentInfo));

  m_currFrame = (m_currFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
