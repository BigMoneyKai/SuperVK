#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>
#include <vector>

class DescriptorPool {
public:
    void init(const VkDevice& device, const std::vector<VkDescriptorPoolSize>& poolSizes, u32 maxSets);
    void destroy();
    VkDescriptorSet allocate(const VkDescriptorSetLayout& layout);

private:
    VkDevice m_device{VK_NULL_HANDLE};
    VkDescriptorPool m_pool{VK_NULL_HANDLE};

};
