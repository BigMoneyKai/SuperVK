#pragma once

#include "core/define/compiler.h"

#include <vulkan/vulkan.h>

class DescriptorSet {
public:
  void init(VkDescriptorSet set);
  SV_FORCE_INLINE VkDescriptorSet set() const { return m_descriptorSet; }

private:
  VkDescriptorSet m_descriptorSet{VK_NULL_HANDLE};
};
