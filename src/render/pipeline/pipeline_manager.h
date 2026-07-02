#pragma once

#include "defines.h"
#include "pipeline.h"
#include "shader.h"

#include <vulkan/vulkan.h>

class PipelineMan {
public:
    void init(const VkDevice& device, const VkRenderPass& renderPass,
              VkDescriptorSetLayout descriptorSetLayout);
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE VkPipeline graphicsPipeline() const { return m_graphics.pipeline(); }
    SV_FORCE_INLINE VkPipelineLayout graphicsPipelineLayout() const { return m_graphics.pipelineLayout(); }

private:
    Shader   m_vertShader;
    Shader   m_fragShader;
    Pipeline m_graphics;
};
