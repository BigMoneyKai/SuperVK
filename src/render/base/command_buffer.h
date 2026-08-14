#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"

#include <vector>
#include <vulkan/vulkan.h>

class CommandBuffer {
public:
  void init(const VkDevice &device, VkCommandPool pool, u32 count);
  void destroy();

  SV_FORCE_INLINE VkCommandBuffer get(u32 index) const {
    return m_commandBuffers.at(index);
  }

private:
  VkDevice m_device{VK_NULL_HANDLE};
  std::vector<VkCommandBuffer> m_commandBuffers;
  VkCommandBufferAllocateInfo m_commandBufferAllocateInfo{};
  VkCommandPool m_pool{VK_NULL_HANDLE};
};
