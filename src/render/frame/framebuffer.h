#pragma once

#include "core/define/compiler.h"
#include <vulkan/vulkan.h>

class Framebuffer {
public:
  void init(const VkDevice &device, const VkRenderPass &renderPass,
            const VkImageView &colorImageView,
            const VkImageView &depthImageView, VkExtent2D extent);
  void destroy();

  SV_FORCE_INLINE const VkFramebuffer &framebuffer() const {
    return m_framebuffer;
  }

private:
  VkDevice m_device{VK_NULL_HANDLE};
  VkFramebuffer m_framebuffer{VK_NULL_HANDLE};
};
