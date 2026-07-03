#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "defines.h"

namespace material {
SV_INLINE constexpr glm::vec3 defaultColor = {1.f, 1.f, 1.f};
SV_INLINE constexpr f32 defaultMetallic = 0.f;
SV_INLINE constexpr f32 defaultRoughness = 1.f;
SV_INLINE constexpr f32 defaultShininess = 32.f;

struct MaterialUBO {
    glm::vec3 color{defaultColor};
    f32 metallic{defaultMetallic};
    f32 roughness{defaultRoughness};
    f32 shininess{defaultShininess};
    f32 pad[2]{};  // std140 struct alignment = 16, total = 32
};

static_assert(sizeof(MaterialUBO) == 32, "MaterialUBO std140 size mismatch");

class Material {
public:
    void init();
    void update();
    void destroy();

    SV_FORCE_INLINE const MaterialUBO& material() const {
        return m_ubo;
    }
    SV_FORCE_INLINE MaterialUBO& material() {
        return m_ubo;
    }

    SV_FORCE_INLINE const MaterialUBO& ubo() const {
        return m_ubo;
    }
    SV_FORCE_INLINE MaterialUBO& ubo() {
        return m_ubo;
    }

private:
    MaterialUBO m_ubo{};
};

} // namespace

using Material = material::Material;
using MaterialUBO = material::MaterialUBO;
