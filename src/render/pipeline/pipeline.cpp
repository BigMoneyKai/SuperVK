#include "pipeline.h"
#include "debug/debugger.h"
#include "core/types/vertex.h"

#include <array>
#include <cstddef>

void Pipeline::init(
    const VkDevice& device,
    const VkRenderPass& renderPass,
    const PipelineDesc& desc)
{
    m_device = device;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<u32>(desc.descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = desc.descriptorSetLayouts.empty() ? nullptr : desc.descriptorSetLayouts.data();

    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout));

    //---------------- Shader Stage ----------------

    std::vector<VkPipelineShaderStageCreateInfo> stages;

    for (auto shader : desc.shaders) {
        VkPipelineShaderStageCreateInfo stage{};
        stage.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

        stage.stage = shader->stage();
        stage.module = shader->module();
        stage.pName = "main";

        stages.push_back(stage);
    }

    //---------------- Vertex Input ----------------

    VkVertexInputBindingDescription vertexInputBindingDescription{};
    vertexInputBindingDescription.binding = 0;
    vertexInputBindingDescription.stride = sizeof(Vertex);
    vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    std::array<VkVertexInputAttributeDescription, 4> vertexAttributes{};
    vertexAttributes[0].binding = 0;
    vertexAttributes[0].location = 0;
    vertexAttributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributes[0].offset = offsetof(Vertex, pos);
    vertexAttributes[1].binding = 0;
    vertexAttributes[1].location = 1;
    vertexAttributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributes[1].offset = offsetof(Vertex, color);
    vertexAttributes[2].binding = 0;
    vertexAttributes[2].location = 2;
    vertexAttributes[2].format = VK_FORMAT_R32G32_SFLOAT;
    vertexAttributes[2].offset = offsetof(Vertex, uv);
    vertexAttributes[3].binding = 0;
    vertexAttributes[3].location = 3;
    vertexAttributes[3].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributes[3].offset = offsetof(Vertex, normal);

    VkPipelineVertexInputStateCreateInfo vertex{};
    vertex.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex.vertexBindingDescriptionCount = 1;
    vertex.pVertexBindingDescriptions = &vertexInputBindingDescription;
    vertex.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributes.size());
    vertex.pVertexAttributeDescriptions = vertexAttributes.data();


    //---------------- Input Assembly ----------------

    VkPipelineInputAssemblyStateCreateInfo assembly{};
    assembly.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

    assembly.topology = desc.topology;

    //---------------- Viewport ----------------

    VkPipelineViewportStateCreateInfo viewport{};
    viewport.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

    viewport.viewportCount = 1;
    viewport.scissorCount = 1;

    //---------------- Rasterizer ----------------

    VkPipelineRasterizationStateCreateInfo raster{};
    raster.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

    raster.polygonMode = desc.polygonMode;
    raster.cullMode = desc.cullMode;
    raster.frontFace = desc.frontFace;
    raster.lineWidth = 1.f;

    //---------------- MSAA ----------------

    VkPipelineMultisampleStateCreateInfo msaa{};
    msaa.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

    msaa.rasterizationSamples =
        VK_SAMPLE_COUNT_1_BIT;

    //---------------- Depth ----------------

    VkPipelineDepthStencilStateCreateInfo depth{};
    depth.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

    depth.depthTestEnable =
        desc.depthTest;

    depth.depthWriteEnable =
        desc.depthWrite;

    depth.depthCompareOp =
        desc.compareOp;
    depth.depthBoundsTestEnable = VK_FALSE;
    depth.stencilTestEnable = VK_FALSE;

    //---------------- Blend ----------------

    VkPipelineColorBlendAttachmentState attachment{};
    attachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;

    attachment.blendEnable =
        desc.blendingEnabled;

    VkPipelineColorBlendStateCreateInfo blend{};
    blend.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

    blend.attachmentCount = 1;
    blend.pAttachments = &attachment;

    //---------------- Dynamic ----------------

    std::vector<VkDynamicState> dynamics;

    if(desc.dynamicViewport)
        dynamics.push_back(VK_DYNAMIC_STATE_VIEWPORT);

    if(desc.dynamicScissor)
        dynamics.push_back(VK_DYNAMIC_STATE_SCISSOR);

    VkPipelineDynamicStateCreateInfo dynamic{};
    dynamic.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

    dynamic.dynamicStateCount =
        static_cast<uint32_t>(dynamics.size());

    dynamic.pDynamicStates = dynamics.empty() ? nullptr : dynamics.data();

    //---------------- Create ----------------

    VkGraphicsPipelineCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    info.stageCount =
        static_cast<uint32_t>(stages.size());

    info.pStages = stages.data();

    info.pVertexInputState = &vertex;
    info.pInputAssemblyState = &assembly;
    info.pViewportState = &viewport;
    info.pRasterizationState = &raster;
    info.pMultisampleState = &msaa;
    info.pDepthStencilState = &depth;
    info.pColorBlendState = &blend;
    info.pDynamicState = dynamics.empty() ? nullptr : &dynamic;

    info.layout = m_pipelineLayout;
    info.renderPass = renderPass;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(
        device,
        VK_NULL_HANDLE,
        1,
        &info,
        nullptr,
        &m_pipeline));
}

void Pipeline::destroy() {
    vkDestroyPipeline(m_device, m_pipeline, nullptr);
    vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
}
