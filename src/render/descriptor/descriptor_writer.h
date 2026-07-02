#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>

#include <vector>

class DescriptorWriter {
public:
    void writeBuffer(u32 binding, VkDescriptorType type, const VkDescriptorBufferInfo& bufferInfo);
    void build(const VkDevice& device, VkDescriptorSet descriptorSet);

private:
    struct BufferWrite {
        u32 binding;
        VkDescriptorType type;
        VkDescriptorBufferInfo info;
    };

    std::vector<BufferWrite> m_bufferWrites;
};
