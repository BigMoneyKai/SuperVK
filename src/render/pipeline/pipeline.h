#pragma once

#include "defines.h"
#include "shader.h"

#include <vulkan/vulkan.h>
#include <utility>
#include <vector>

struct PipelineDesc {
    // shaders
    std::vector<const Shader*> shaders;

    // input assembly
    VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    // rasterization
    VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
    VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
    VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE;

    // depth
    b32 depthTest = SV_TRUE;
    b32 depthWrite = SV_TRUE;
    VkCompareOp compareOp = VK_COMPARE_OP_LESS;

    // blending
    b32 blendingEnabled = SV_FALSE;

    // dynamic
    b32 dynamicViewport = SV_TRUE;
    b32 dynamicScissor = SV_TRUE;

    // funcs
    SV_FORCE_INLINE void addShader(const Shader* shader) {
        shaders.push_back(shader);
    }

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
};

class Pipeline {
public:
    void init(const VkDevice& device, const VkRenderPass& renderPass, const PipelineDesc& desc);
    void destroy();

    SV_FORCE_INLINE const VkPipeline& pipeline() const {
        return m_pipeline;
    }
    SV_FORCE_INLINE const VkPipelineLayout& pipelineLayout() const {
        return m_pipelineLayout;
    }

private:
    VkDevice m_device{VK_NULL_HANDLE};
    VkPipelineLayout m_pipelineLayout{};
    VkPipeline m_pipeline{};

};

