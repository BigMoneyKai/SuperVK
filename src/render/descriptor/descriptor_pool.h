#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>
#include <vector>

class DescriptorPool {
public:
    void init(const VkDevice& device, const std::vector<VkDescriptorPoolSize>& poolSizes, u32 maxSets);
    void destroy(const VkDevice& device);
    VkDescriptorSet allocate(const VkDevice& device, const VkDescriptorSetLayout& layout);

private:
    VkDescriptorPool m_pool{VK_NULL_HANDLE};

};
