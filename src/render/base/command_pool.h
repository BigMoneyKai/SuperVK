#pragma once

#include <vulkan/vulkan.h>

#include "core/define/compiler.h"
#include "core/define/types.h"

class CommandPool {
public:
  void init(const VkDevice &device, u32 graphicsQueueFamilyIndex);
  void destroy();

  SV_FORCE_INLINE const VkCommandPool &pool() const { return commandPool; }
  SV_FORCE_INLINE VkCommandPool pool() { return commandPool; }

private:
  VkDevice m_device{VK_NULL_HANDLE};
  VkCommandPoolCreateInfo commandPoolCreateInfo{};
  VkCommandPool commandPool{VK_NULL_HANDLE};
};
