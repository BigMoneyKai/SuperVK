#pragma once

#include "defines.h"

#include "descriptor_pool.h"
#include "descriptor_set.h"
#include "descriptor_set_layout.h"

#include "render/resource/buffer.h"

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
// ---------------------------------------------------------------------------
class DescriptorMan {
public:
    void init(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
    void destroy(const VkDevice& device);

    // per-frame UBO upload (delegates to Buffer::update)
    void updateCameraUBO(const VkDevice& device, const void* data, u64 size);
    void updateObjectUBO(const VkDevice& device, const void* data, u64 size);
    void updateLightUBO(const VkDevice& device, const void* data, u64 size);
    void updateMaterialUBO(const VkDevice& device, const void* data, u64 size);

    SV_FORCE_INLINE VkDescriptorSetLayout layout() const { return m_setLayout.layout(); }
    SV_FORCE_INLINE VkDescriptorSet descriptorSet() const { return m_set.set(); }

private:
    void createLayout(const VkDevice& device);
    void createPool(const VkDevice& device);
    void allocateSet(const VkDevice& device);
    void createBuffers(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
    void writeDescriptorSet(const VkDevice& device);

    DescriptorSetLayout m_setLayout;
    DescriptorPool      m_pool;
    DescriptorSet       m_set;

    Buffer m_cameraUBO;
    Buffer m_objectUBO;
    Buffer m_lightUBO;
    Buffer m_materialUBO;
};
