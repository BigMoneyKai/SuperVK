#include "scene/scene.h"

void Scene::init(const VkDevice& device, const VkPhysicalDevice& physicalDevice, u32 width, u32 height) {
    f32 aspect = static_cast<f32>(width) / static_cast<f32>(height);
    m_camera.init(aspect);
    m_object.init();
    m_material.init();
    m_light.init();

    m_mesh.init(device, physicalDevice);
}

void Scene::destroy() {
    m_camera.destroy();
    m_object.destroy();
    m_material.destroy();
    m_light.destroy();
    m_mesh.destroy();
}
