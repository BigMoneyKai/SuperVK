#include "scene/scene.h"

void Scene::init(const VkDevice& device, const VkPhysicalDevice& physicalDevice, u32 width, u32 height) {
    f32 aspect = static_cast<f32>(width) / static_cast<f32>(height);
    m_camera.init(aspect);
    m_object.init();
    m_material.init();
    m_light.init();

    m_mesh.init(device, physicalDevice);
}

void Scene::destroy(const VkDevice& device) {
    m_camera.destroy(device);
    m_object.destroy(device);
    m_material.destroy(device);
    m_light.destroy(device);
    m_mesh.destroy(device);
}
