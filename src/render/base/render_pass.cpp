#include "render_pass.h"
#include "debug/debugger.h"

#include <array>

void RenderPass::init(const VkDevice& device, VkFormat colorFormat, VkFormat depthFormat) {
    m_device = device;
    m_colorAttachment.format = colorFormat;
    m_colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    m_colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    m_colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    m_colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    m_colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    m_colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    m_colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    m_depthAttachment.format = depthFormat;
    m_depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    m_depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    m_depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    m_depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    m_depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    m_depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    m_depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    m_colorAttachmentRef.attachment = 0;
    m_colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    m_depthAttachmentRef.attachment = 1;
    m_depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    m_subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    m_subpass.colorAttachmentCount = 1;
    m_subpass.pColorAttachments = &m_colorAttachmentRef;
    m_subpass.pDepthStencilAttachment = &m_depthAttachmentRef;

    m_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    m_dependency.dstSubpass = 0;
    m_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    m_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    m_dependency.srcAccessMask = 0;
    m_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    m_renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    std::array<VkAttachmentDescription, 2> attachments = {m_colorAttachment, m_depthAttachment};
    m_renderPassCreateInfo.attachmentCount = static_cast<u32>(attachments.size());
    m_renderPassCreateInfo.pAttachments = attachments.data();
    m_renderPassCreateInfo.subpassCount = 1;
    m_renderPassCreateInfo.pSubpasses = &m_subpass;
    m_renderPassCreateInfo.dependencyCount = 1;
    m_renderPassCreateInfo.pDependencies = &m_dependency;

    VK_CHECK_RESULT(vkCreateRenderPass(device, &m_renderPassCreateInfo, nullptr, &m_renderPass));

}

void RenderPass::destroy() {
    if(m_renderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);
        m_renderPass = VK_NULL_HANDLE;
    }
}
