#include "descriptor_manager.h"

#include "descriptor_writer.h"

#include "scene/camera.h"
#include "scene/object.h"
#include "scene/light.h"
#include "scene/material.h"

void DescriptorMan::init(const VkDevice& device, const VkPhysicalDevice& physicalDevice) {
    m_device = device;
    createLayout();
    createPool();
    allocateSet();
    createBuffers(physicalDevice);
}

void DescriptorMan::destroy() {
    m_cameraUBO.destroy();
    m_objectUBO.destroy();
    m_lightUBO.destroy();
    m_materialUBO.destroy();
    m_texSampler2D.destroy();
    m_pool.destroy();
    m_setLayout.destroy();
}

// ---- UBO updates -----------------------------------------------------------

void DescriptorMan::updateCameraUBO(const void* data, u64 size) {
    m_cameraUBO.update(data, size);
}

void DescriptorMan::updateObjectUBO(const void* data, u64 size) {
    m_objectUBO.update(data, size);
}

void DescriptorMan::updateLightUBO(const void* data, u64 size) {
    m_lightUBO.update(data, size);
}

void DescriptorMan::updateMaterialUBO(const void* data, u64 size) {
    m_materialUBO.update(data, size);
}

// ---- internal helpers ------------------------------------------------------

void DescriptorMan::createLayout() {
    std::vector<VkDescriptorSetLayoutBinding> bindings(5);

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

    // binding 4 — Sampler2D
    bindings[4].binding            = 4;
    bindings[4].descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[4].descriptorCount    = 1;
    bindings[4].stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings[4].pImmutableSamplers = nullptr;

    m_setLayout.init(m_device, bindings);
}

void DescriptorMan::createPool() {
    std::vector<VkDescriptorPoolSize> poolSizes = {
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1}
    };
    m_pool.init(m_device, poolSizes, 1);
}

void DescriptorMan::allocateSet() {
    VkDescriptorSet set = m_pool.allocate(m_setLayout.layout());
    m_set.init(set);
}

void DescriptorMan::createBuffers(const VkPhysicalDevice& physicalDevice) {
    m_cameraUBO.init(BT_UNIFORM_BUFFER, m_device, physicalDevice, nullptr, sizeof(CameraUBO));
    m_objectUBO.init(BT_UNIFORM_BUFFER, m_device, physicalDevice, nullptr, sizeof(ObjectUBO));
    m_lightUBO.init(BT_UNIFORM_BUFFER, m_device, physicalDevice, nullptr, sizeof(LightUBO));
    m_materialUBO.init(BT_UNIFORM_BUFFER, m_device, physicalDevice, nullptr, sizeof(MaterialUBO));
    m_texSampler2D.init(m_device, {});
}

void DescriptorMan::writeBufferDescriptorSet() {
    DescriptorWriter writer;
    writer.init(m_device);

    VkDescriptorBufferInfo camInfo{};
    camInfo.buffer = m_cameraUBO.buffer();
    camInfo.offset = 0;
    camInfo.range  = VK_WHOLE_SIZE;
    writer.writeBuffer({0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, camInfo});

    VkDescriptorBufferInfo objInfo{};
    objInfo.buffer = m_objectUBO.buffer();
    objInfo.offset = 0;
    objInfo.range  = VK_WHOLE_SIZE;
    writer.writeBuffer({1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, objInfo});

    VkDescriptorBufferInfo lightInfo{};
    lightInfo.buffer = m_lightUBO.buffer();
    lightInfo.offset = 0;
    lightInfo.range  = VK_WHOLE_SIZE;
    writer.writeBuffer({2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, lightInfo});

    VkDescriptorBufferInfo matInfo{};
    matInfo.buffer = m_materialUBO.buffer();
    matInfo.offset = 0;
    matInfo.range  = VK_WHOLE_SIZE;
    writer.writeBuffer({3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, matInfo});

    writer.buildBufferWrite(m_set.set());
}

void DescriptorMan::writeImageDescriptorSet(const VkImageView& imageView, const VkImageLayout& layout) {
    DescriptorWriter writer;
    writer.init(m_device);

    VkDescriptorImageInfo texInfo{};
    texInfo.sampler     = m_texSampler2D.sampler();
    texInfo.imageView   = imageView;
    texInfo.imageLayout = layout;
    writer.writeImage({4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, texInfo});

    writer.buildImageWrite(m_set.set());
}
