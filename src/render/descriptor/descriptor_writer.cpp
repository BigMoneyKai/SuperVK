#include "descriptor_writer.h"

void DescriptorWriter::init(const VkDevice& device) {
    m_device = device;
}

void DescriptorWriter::writeBuffer(const BufferWriteDesc& desc) {
    m_bufferWriteDescs.push_back(desc);
}

void DescriptorWriter::writeImage(const ImageWriteDesc& desc) {
    m_imageWriteDescs.push_back(desc);
}

void DescriptorWriter::buildImageWrite(VkDescriptorSet descriptorSet) {
    std::vector<VkWriteDescriptorSet> imageDescSets;
    imageDescSets.reserve(m_imageWriteDescs.size());

    for(auto& imageWriteDesc : m_imageWriteDescs) {
        VkWriteDescriptorSet imageWrite{};
        imageWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        imageWrite.dstSet = descriptorSet;
        imageWrite.dstBinding = imageWriteDesc.binding;
        imageWrite.dstArrayElement = 0;
        imageWrite.descriptorCount = 1;
        imageWrite.descriptorType = imageWriteDesc.type;
        imageWrite.pImageInfo = &imageWriteDesc.info;

        imageDescSets.push_back(imageWrite);
    }
    vkUpdateDescriptorSets(
        m_device,
        static_cast<u32>(imageDescSets.size()),
        imageDescSets.data(),
        0,
        nullptr
    );
}

void DescriptorWriter::buildBufferWrite(VkDescriptorSet descriptorSet) {
    std::vector<VkWriteDescriptorSet> bufferDescSets;
    bufferDescSets.reserve(m_bufferWriteDescs.size());

    for(auto& bufferWriteDesc : m_bufferWriteDescs) {
        VkWriteDescriptorSet bufferWrite{};
        bufferWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        bufferWrite.dstSet = descriptorSet;
        bufferWrite.dstBinding = bufferWriteDesc.binding;
        bufferWrite.dstArrayElement = 0;
        bufferWrite.descriptorCount = 1;
        bufferWrite.descriptorType = bufferWriteDesc.type;
        bufferWrite.pBufferInfo = &bufferWriteDesc.info;

        bufferDescSets.push_back(bufferWrite);
    }
    vkUpdateDescriptorSets(
        m_device,
        static_cast<u32>(bufferDescSets.size()),
        bufferDescSets.data(),
        0,
        nullptr
    );


}
