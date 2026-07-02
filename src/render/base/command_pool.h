#pragma once

#include <vulkan/vulkan.h>

#include "defines.h"
#include "debug/debug.h"
#include "device.h"

#include <vector>

class CommandPool {
public:
    void init(const VkDevice& device, u32 graphicsQueueFamilyIndex);
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE const VkCommandPool& pool() const {
        return commandPool;
    }
    SV_FORCE_INLINE VkCommandPool pool() {
        return commandPool;
    }

private:
    VkCommandPoolCreateInfo commandPoolCreateInfo{};
    VkCommandPool commandPool{VK_NULL_HANDLE};
    
};
