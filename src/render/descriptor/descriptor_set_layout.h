#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>
#include <vector>

class DescriptorSetLayout {
public:
    void init(const VkDevice& device, const std::vector<VkDescriptorSetLayoutBinding>& bindings);
    void destroy();

    SV_FORCE_INLINE VkDescriptorSetLayout layout() const {
        return m_layout;
    }
private:
    VkDevice m_device{VK_NULL_HANDLE};
    VkDescriptorSetLayout m_layout{VK_NULL_HANDLE};

};
