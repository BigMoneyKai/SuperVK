#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"

#include "descriptor_pool.h"
#include "descriptor_set.h"
#include "descriptor_set_layout.h"
#include "render/frame/sync.h"

#include "render/resource/buffer.h"
#include "render/resource/sampler.h"

#include <array>
#include <vulkan/vulkan.h>

// ---------------------------------------------------------------------------
// DescriptorMan — manages the global descriptor set layout, pool, set, and
//                 4 uniform buffers for the scene UBOs.
//
// UBO binding layout (matches shaders/glsl/include/*.glsl):
//   binding 0 — CameraUBO
//   binding 1 — ObjectUBO
//   binding 2 — LightUBO
//   binding 3 — MaterialUBO
//   binding 4 — Sampler2D
// ---------------------------------------------------------------------------
class DescriptorMan {
public:
  void init(const VkDevice &device, const VkPhysicalDevice &physicalDevice);
  void destroy();

  // per-frame UBO upload (delegates to Buffer::update)
  void updateCameraUBO(u32 frame, const void *data, u64 size);
  void updateObjectUBO(u32 frame, const void *data, u64 size);
  void updateLightUBO(u32 frame, const void *data, u64 size);
  void updateMaterialUBO(u32 frame, const void *data, u64 size);

  void writeBufferDescriptorSet();
  void writeImageDescriptorSet(u32 frame, const VkImageView &imageView,
                               const VkImageLayout &layout);

  SV_FORCE_INLINE VkDescriptorSetLayout layout() const {
    return m_setLayout.layout();
  }
  SV_FORCE_INLINE VkDescriptorSet descriptorSet(u32 frame) const {
    return m_sets[frame].set();
  }

private:
  void createLayout();
  void createPool();
  void allocateSet();
  void createBuffers(const VkPhysicalDevice &physicalDevice);

  VkDevice m_device{VK_NULL_HANDLE};

  static constexpr u32 kFrames = MAX_FRAMES_IN_FLIGHT;
  DescriptorSetLayout m_setLayout;
  DescriptorPool m_pool;

  std::array<DescriptorSet, kFrames> m_sets;

  std::array<Buffer, kFrames> m_cameraUBOs;
  std::array<Buffer, kFrames> m_objectUBOs;
  std::array<Buffer, kFrames> m_lightUBOs;
  std::array<Buffer, kFrames> m_materialUBOs;
  Sampler m_texSampler2D;
};
