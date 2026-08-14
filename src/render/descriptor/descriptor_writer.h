#pragma once

#include "core/define/types.h"

#include <vector>
#include <vulkan/vulkan.h>

struct ImageWriteDesc {
  u32 binding;
  VkDescriptorType type;
  VkDescriptorImageInfo info;
};

struct BufferWriteDesc {
  u32 binding;
  VkDescriptorType type;
  VkDescriptorBufferInfo info;
};

class DescriptorWriter {
public:
  void init(const VkDevice &device);

  void writeBuffer(const BufferWriteDesc &desc);
  void writeImage(const ImageWriteDesc &desc);

  void buildBufferWrite(VkDescriptorSet descriptorSet);
  void buildImageWrite(VkDescriptorSet descriptorSet);

private:
  VkDevice m_device{VK_NULL_HANDLE};
  std::vector<BufferWriteDesc> m_bufferWriteDescs;
  std::vector<ImageWriteDesc> m_imageWriteDescs;
};
