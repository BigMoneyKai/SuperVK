#pragma once

#include "core/define/compiler.h"
#include "scene/camera.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/mesh.h"
#include "scene/object.h"

#include <vulkan/vulkan.h>

class Scene {
public:
  void init(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
            u32 width, u32 height);
  void destroy();

  SV_FORCE_INLINE Camera &camera() { return m_camera; }
  SV_FORCE_INLINE Object &object() { return m_object; }
  SV_FORCE_INLINE Material &material() { return m_material; }
  SV_FORCE_INLINE Light &light() { return m_light; }
  SV_FORCE_INLINE Mesh &mesh() { return m_mesh; }

private:
  Camera m_camera;
  Object m_object;
  Material m_material;
  Light m_light;
  Mesh m_mesh;
};
