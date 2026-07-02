#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>
#include <vector>

class Framebuffer {
public:
    void init(
        const VkDevice& device,
        const VkRenderPass& renderPass,
        const VkImageView& colorImageView,
        const VkImageView& depthImageView,
        VkExtent2D extent
    );
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE const VkFramebuffer& framebuffer() const {
        return m_framebuffer;
    }

private:
    VkFramebuffer m_framebuffer{VK_NULL_HANDLE};
};
