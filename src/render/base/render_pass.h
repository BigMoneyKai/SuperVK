#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>

class RenderPass {
public:
    void init(const VkDevice& device, VkFormat colorFormat, VkFormat depthFormat);
    void destroy();

    SV_FORCE_INLINE const VkRenderPass& renderPass() const {
        return m_renderPass;
    }

private:
    VkDevice m_device{VK_NULL_HANDLE};
    VkAttachmentDescription m_colorAttachment{};
    VkAttachmentDescription m_depthAttachment{};
    VkAttachmentReference m_colorAttachmentRef{}; 
    VkAttachmentReference m_depthAttachmentRef{};
    VkSubpassDescription m_subpass{};
    VkSubpassDependency m_dependency{};
    VkRenderPassCreateInfo m_renderPassCreateInfo{};
    VkRenderPass m_renderPass{VK_NULL_HANDLE};

};
