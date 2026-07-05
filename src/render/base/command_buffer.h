#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>

#include <vector>

class CommandBuffer {
public:
    void init(const VkDevice& device, VkCommandPool pool, u32 count);
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
