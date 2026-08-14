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
  m_renderPass.init(m_device.device(), m_swapchain.colorFormat(),
                    m_device.depthFormat());

  // DescriptorMan must init BEFORE PipelineMan (pipeline needs the layout)
  m_descriptorMan.init(m_device.device(), m_device.physicalDevice());
  m_pipelineMan.init(m_device.device(), m_renderPass.renderPass(),
                     m_descriptorMan.layout());
  m_bufferMan.init();

  m_frameResource.init(m_device.device(), m_swapchain.imageCount());
  m_frameResource.createSyncPrimitives();
  m_depthResource.init(m_device.device(), m_device.physicalDevice(),
                       m_device.depthFormat(), m_swapchain.extent());

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

  const auto &swapchainImageViews = m_swapchain.swapchainImageViews();
  m_framebuffers.resize(swapchainImageViews.size());

  for (size_t i = 0; i < swapchainImageViews.size(); ++i) {
    m_framebuffers[i].init(
        m_device.device(), m_renderPass.renderPass(), swapchainImageViews[i],
        m_depthResource.depthImageView(), m_swapchain.extent());
  }

  DEBUG(LogCatag::render, "Renderer initialized");
}

void Renderer::render(Scene &scene) { drawFrame(scene); }

void Renderer::waitIdle() { vkDeviceWaitIdle(m_device.device()); }

void Renderer::destroy() {
  // framebuffers
  for (auto &fb : m_framebuffers) {
    fb.destroy();
  }
  m_framebuffers.clear();

  // sync, depth, commands
  m_frameResource.destroy();
  m_depthResource.destroy();

  m_textureMan.destroy();

  m_commandPool.destroy();

  // pipeline + descriptors
  m_pipelineMan.destroy();
  m_descriptorMan.destroy();

  // render pass, swapchain, surface, device, instance
  m_renderPass.destroy();
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

  VkRenderPassBeginInfo renderPassBeginInfo{};
  renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassBeginInfo.renderPass = m_renderPass.renderPass();
  renderPassBeginInfo.framebuffer = m_framebuffers[m_imageIndex].framebuffer();
  renderPassBeginInfo.renderArea.offset = {0, 0};
  renderPassBeginInfo.renderArea.extent = m_swapchain.extent();

  VkClearValue clearValues[2]{};
  clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassBeginInfo.clearValueCount = 2;
  renderPassBeginInfo.pClearValues = clearValues;

  vkCmdBeginRenderPass(cmdBuf, &renderPassBeginInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  // ---- update UBOs from scene data ----
  m_descriptorMan.updateCameraUBO(m_currFrame, &scene.camera().ubo(),
                                  sizeof(CameraUBO));
  m_descriptorMan.updateObjectUBO(m_currFrame, &scene.object().ubo(),
                                  sizeof(ObjectUBO));
  m_descriptorMan.updateLightUBO(m_currFrame, &scene.light().ubo(),
                                 sizeof(LightUBO));
  m_descriptorMan.updateMaterialUBO(m_currFrame, &scene.material().ubo(),
                                    sizeof(MaterialUBO));

  // ---- bind pipeline ----
  vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    m_pipelineMan.graphicsPipeline());

  // ---- bind descriptor set ----
  VkDescriptorSet ds = m_descriptorMan.descriptorSet(m_currFrame);
  vkCmdBindDescriptorSets(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          m_pipelineMan.graphicsPipelineLayout(), 0, 1, &ds, 0,
                          nullptr);

  // ---- dynamic viewport + scissor ----
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<f32>(m_swapchain.extent().width);
  viewport.height = static_cast<f32>(m_swapchain.extent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(cmdBuf, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = m_swapchain.extent();
  vkCmdSetScissor(cmdBuf, 0, 1, &scissor);

  // ---- bind vertex + index buffers ----
  VkBuffer vb = scene.mesh().vertexBuffer().buffer();
  VkDeviceSize vbOffsets[1] = {0};
  vkCmdBindVertexBuffers(cmdBuf, 0, 1, &vb, vbOffsets);
  vkCmdBindIndexBuffer(cmdBuf, scene.mesh().indexBuffer().buffer(), 0,
                       VK_INDEX_TYPE_UINT32);

  // ---- draw ----
  u32 indexCount = static_cast<u32>(scene.mesh().indices().size());
  vkCmdDrawIndexed(cmdBuf, indexCount, 1, 0, 0, 0);

  vkCmdEndRenderPass(cmdBuf);

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
