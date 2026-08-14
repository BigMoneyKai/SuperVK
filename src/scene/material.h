#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "core/define/compiler.h"
#include "core/define/types.h"

namespace material {
SV_INLINE constexpr glm::vec3 defaultColor = {1.f, 1.f, 1.f};
SV_INLINE constexpr f32 defaultMetallic = 0.f;
SV_INLINE constexpr f32 defaultRoughness = 1.f;
SV_INLINE constexpr f32 defaultShininess = 32.f;

struct MaterialUBO {
  glm::vec3 color{defaultColor};
  f32 _pad0{}; // std140: vec3 → 16 bytes
  f32 metallic{defaultMetallic};
  f32 roughness{defaultRoughness};
  f32 shininess{defaultShininess};
  f32 pad[1]{}; // struct aligned to 32 bytes
};

static_assert(sizeof(MaterialUBO) == 32, "MaterialUBO std140 size mismatch");
static_assert(offsetof(MaterialUBO, color) == 0, "color offset");
static_assert(offsetof(MaterialUBO, metallic) == 16, "metallic offset");
static_assert(offsetof(MaterialUBO, roughness) == 20, "roughness offset");
static_assert(offsetof(MaterialUBO, shininess) == 24, "shininess offset");

class Material {
public:
  void init();
  void update();
  void destroy();

  SV_FORCE_INLINE const MaterialUBO &material() const { return m_ubo; }
  SV_FORCE_INLINE MaterialUBO &material() { return m_ubo; }

  SV_FORCE_INLINE const MaterialUBO &ubo() const { return m_ubo; }
  SV_FORCE_INLINE MaterialUBO &ubo() { return m_ubo; }

private:
  MaterialUBO m_ubo{};
};

} // namespace material

using Material = material::Material;
using MaterialUBO = material::MaterialUBO;
