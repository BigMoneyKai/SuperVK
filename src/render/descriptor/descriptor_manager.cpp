#include "descriptor_manager.h"

#include "descriptor_writer.h"

#include "scene/camera.h"
#include "scene/object.h"
#include "scene/light.h"
#include "scene/material.h"

void DescriptorMan::init(const VkDevice& device, const VkPhysicalDevice& physicalDevice) {
    createLayout(device);
    createPool(device);
    allocateSet(device);
    createBuffers(device, physicalDevice);
    writeDescriptorSet(device);
}

void DescriptorMan::destroy(const VkDevice& device) {
    m_cameraUBO.destroy(device);
    m_objectUBO.destroy(device);
    m_lightUBO.destroy(device);
    m_materialUBO.destroy(device);
    m_pool.destroy(device);
    m_setLayout.destroy(device);
}

// ---- UBO updates -----------------------------------------------------------

void DescriptorMan::updateCameraUBO(const VkDevice& device, const void* data, u64 size) {
    m_cameraUBO.update(device, data, size);
}

void DescriptorMan::updateObjectUBO(const VkDevice& device, const void* data, u64 size) {
    m_objectUBO.update(device, data, size);
}

void DescriptorMan::updateLightUBO(const VkDevice& device, const void* data, u64 size) {
    m_lightUBO.update(device, data, size);
}

void DescriptorMan::updateMaterialUBO(const VkDevice& device, const void* data, u64 size) {
    m_materialUBO.update(device, data, size);
}

// ---- internal helpers ------------------------------------------------------

void DescriptorMan::createLayout(const VkDevice& device) {
    std::vector<VkDescriptorSetLayoutBinding> bindings(4);

    // binding 0 — CameraUBO
    bindings[0].binding            = 0;
    bindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].descriptorCount    = 1;
    bindings[0].stageFlags         = VK_SHADER_STAGE_ALL_GRAPHICS;
    bindings[0].pImmutableSamplers = nullptr;

    // binding 1 — ObjectUBO
    bindings[1].binding            = 1;
    bindings[1].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[1].descriptorCount    = 1;
    bindings[1].stageFlags         = VK_SHADER_STAGE_ALL_GRAPHICS;
    bindings[1].pImmutableSamplers = nullptr;

    // binding 2 — LightUBO
    bindings[2].binding            = 2;
    bindings[2].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[2].descriptorCount    = 1;
    bindings[2].stageFlags         = VK_SHADER_STAGE_ALL_GRAPHICS;
    bindings[2].pImmutableSamplers = nullptr;

    // binding 3 — MaterialUBO
    bindings[3].binding            = 3;
    bindings[3].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[3].descriptorCount    = 1;
    bindings[3].stageFlags         = VK_SHADER_STAGE_ALL_GRAPHICS;
    bindings[3].pImmutableSamplers = nullptr;

    m_setLayout.init(device, bindings);
}

void DescriptorMan::createPool(const VkDevice& device) {
    std::vector<VkDescriptorPoolSize> poolSizes = {
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4}
    };
    m_pool.init(device, poolSizes, 1);
}

void DescriptorMan::allocateSet(const VkDevice& device) {
    VkDescriptorSet set = m_pool.allocate(device, m_setLayout.layout());
    m_set.init(set);
}

void DescriptorMan::createBuffers(const VkDevice& device, const VkPhysicalDevice& physicalDevice) {
    m_cameraUBO.init(BT_UNIFORM_BUFFER, device, physicalDevice, nullptr, sizeof(CameraUBO));
    m_objectUBO.init(BT_UNIFORM_BUFFER, device, physicalDevice, nullptr, sizeof(ObjectUBO));
    m_lightUBO.init(BT_UNIFORM_BUFFER, device, physicalDevice, nullptr, sizeof(LightUBO));
    m_materialUBO.init(BT_UNIFORM_BUFFER, device, physicalDevice, nullptr, sizeof(MaterialUBO));
}

void DescriptorMan::writeDescriptorSet(const VkDevice& device) {
    DescriptorWriter writer;

    VkDescriptorBufferInfo camInfo{};
    camInfo.buffer = m_cameraUBO.buffer();
    camInfo.offset = 0;
    camInfo.range  = VK_WHOLE_SIZE;
    writer.writeBuffer(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, camInfo);

    VkDescriptorBufferInfo objInfo{};
    objInfo.buffer = m_objectUBO.buffer();
    objInfo.offset = 0;
    objInfo.range  = VK_WHOLE_SIZE;
    writer.writeBuffer(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, objInfo);

    VkDescriptorBufferInfo lightInfo{};
    lightInfo.buffer = m_lightUBO.buffer();
    lightInfo.offset = 0;
    lightInfo.range  = VK_WHOLE_SIZE;
    writer.writeBuffer(2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, lightInfo);

    VkDescriptorBufferInfo matInfo{};
    matInfo.buffer = m_materialUBO.buffer();
    matInfo.offset = 0;
    matInfo.range  = VK_WHOLE_SIZE;
    writer.writeBuffer(3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, matInfo);

    writer.build(device, m_set.set());
}
