#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>
#include <vector>

class DescriptorSetLayout {
public:
    void init(const VkDevice& device, const std::vector<VkDescriptorSetLayoutBinding>& bindings);
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE VkDescriptorSetLayout layout() const {
        return m_layout;
    }
private:
    VkDescriptorSetLayout m_layout{VK_NULL_HANDLE};

};
