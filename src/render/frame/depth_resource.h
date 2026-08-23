#pragma once

#include "core/define/compiler.h"
#include <vulkan/vulkan.h>

class DepthResource {
public:
  void init(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
            VkFormat depthFormat, VkExtent2D extent);
  void destroy();

  SV_FORCE_INLINE const VkImageView &depthImageView() const {
    return m_depthImageView;
  }
  SV_FORCE_INLINE const VkImage &depthImage() const { return m_depthImage; }

private:
  VkDevice m_device{VK_NULL_HANDLE};
  VkImage m_depthImage{VK_NULL_HANDLE};
  VkImageView m_depthImageView{VK_NULL_HANDLE};
  VkDeviceMemory m_depthImageMemory{VK_NULL_HANDLE};
};
