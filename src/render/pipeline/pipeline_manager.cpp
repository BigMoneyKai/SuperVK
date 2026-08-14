#include "pipeline_manager.h"

void PipelineMan::init(const VkDevice &device, const VkRenderPass &renderPass,
                       VkDescriptorSetLayout descriptorSetLayout) {
  m_device = device;
  // load shaders
  m_vertShader.init(device, "shaders/spv/pbr.vert.spv", ShaderType::vertexShader);
  m_fragShader.init(device, "shaders/spv/pbr.frag.spv", ShaderType::fragmentShader);

  // build pipeline descriptor
  PipelineDesc desc;
  desc.addShader(&m_vertShader);
  desc.addShader(&m_fragShader);
  desc.descriptorSetLayouts = {descriptorSetLayout};

  // defaults are fine: triangle list, fill, back-face cull, clockwise,
  // depth test/write on, dynamic viewport/scissor on

  m_graphics.init(device, renderPass, desc);
}

void PipelineMan::destroy() {
  m_graphics.destroy();
  m_vertShader.destroy();
  m_fragShader.destroy();
}
