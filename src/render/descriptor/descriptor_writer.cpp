#include "descriptor_writer.h"

void DescriptorWriter::writeBuffer(u32 binding, VkDescriptorType type, const VkDescriptorBufferInfo& bufferInfo) {
    m_bufferWrites.push_back({binding, type, bufferInfo});
}

void DescriptorWriter::build(const VkDevice& device, VkDescriptorSet descriptorSet) {
    std::vector<VkWriteDescriptorSet> writes;
    writes.reserve(m_bufferWrites.size());

    for(auto& bufferWrite : m_bufferWrites) {
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = descriptorSet;
        write.dstBinding = bufferWrite.binding;
        write.dstArrayElement = 0;
        write.descriptorCount = 1;
        write.descriptorType = bufferWrite.type;
        write.pBufferInfo = &bufferWrite.info;
        writes.push_back(write);
    }

    vkUpdateDescriptorSets(
        device,
        static_cast<u32>(writes.size()),
        writes.data(),
        0,
        nullptr
    );
}
