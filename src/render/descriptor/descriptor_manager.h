#pragma once

#include "defines.h"

#include "descriptor_pool.h"
#include "descriptor_set.h"
#include "descriptor_set_layout.h"

#include "render/resource/buffer.h"
#include "render/resource/sampler.h"

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
    void init(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
    void destroy();

    // per-frame UBO upload (delegates to Buffer::update)
    void updateCameraUBO(const void* data, u64 size);
    void updateObjectUBO(const void* data, u64 size);
    void updateLightUBO(const void* data, u64 size);
    void updateMaterialUBO(const void* data, u64 size);

    void writeBufferDescriptorSet();
    void writeImageDescriptorSet(const VkImageView& imageView, const VkImageLayout& layout);

    SV_FORCE_INLINE VkDescriptorSetLayout layout() const { return m_setLayout.layout(); }
    SV_FORCE_INLINE VkDescriptorSet descriptorSet() const { return m_set.set(); }

private:
    void createLayout();
    void createPool();
    void allocateSet();
    void createBuffers(const VkPhysicalDevice& physicalDevice);

    VkDevice m_device{VK_NULL_HANDLE};

    DescriptorSetLayout m_setLayout;
    DescriptorPool      m_pool;
    DescriptorSet       m_set;

    Buffer m_cameraUBO;
    Buffer m_objectUBO;
    Buffer m_lightUBO;
    Buffer m_materialUBO;
    Sampler m_texSampler2D;
};
