#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "defines.h"

namespace light {

SV_INLINE constexpr f32 defaultStrength = 0.15f;
SV_INLINE constexpr glm::vec3 defaultDir = {0.f, -1.f, 0.f};
SV_INLINE constexpr glm::vec3 defaultPos = {0.f, 0.f, 0.f};
SV_INLINE constexpr glm::vec3 defaultColor = {1.f, 1.f, 1.f};
SV_INLINE constexpr f32 defaultConstant = 1.f;
SV_INLINE constexpr f32 defaultLinear = 0.09f;
SV_INLINE constexpr f32 defaultQuadratic = 0.032f;
SV_INLINE constexpr f32 defaultIntensity = 1.f;
SV_INLINE constexpr f32 defaultInnerCutoff = 1.f;
SV_INLINE constexpr f32 defaultOuterCutoff = 1.f;

// ---- std140-compliant light structs ----
// All vec3 members are manually padded to 16-byte boundaries.
// Sizes verified by static_assert below.

struct AmbientLight {
    f32 strength{defaultStrength};   // offset 0
    f32 pad0[3]{};                   // offset 4  (align next vec3 to 16)
    glm::vec3 color{defaultColor};   // offset 16
    f32 pad1{0.0f};                  // offset 28 (array stride padding)
};
// size = 32

struct DirectionalLight {
    glm::vec3 dir{defaultDir};       // offset 0
    f32 _pad0{0.0f};                 // offset 12 (align next vec3 to 16)
    glm::vec3 color{defaultColor};   // offset 16
    f32 intensity{defaultIntensity}; // offset 28
};
// size = 32

struct PointLight {
    glm::vec3 pos{defaultPos};       // offset 0
    f32 _pad0{0.0f};                 // offset 12 (align next vec3 to 16)
    glm::vec3 color{defaultColor};   // offset 16
    f32 intensity{defaultIntensity}; // offset 28
    f32 constant{defaultConstant};   // offset 32
    f32 linear{defaultLinear};       // offset 36
    f32 quadratic{defaultQuadratic}; // offset 40
    f32 _pad1{0.0f};                 // offset 44 (array stride = 48)
};
// size = 48

struct SpotLight {
    glm::vec3 pos{defaultPos};               // offset 0
    f32 _pad0{0.0f};                         // offset 12
    glm::vec3 dir{defaultDir};               // offset 16
    f32 _pad1{0.0f};                         // offset 28
    glm::vec3 color{defaultColor};           // offset 32
    f32 _pad2{0.0f};                         // offset 44
    f32 intensity{defaultIntensity};         // offset 48
    f32 constant{defaultConstant};           // offset 52
    f32 linear{defaultLinear};               // offset 56
    f32 quadratic{defaultQuadratic};         // offset 60
    f32 innerCutoff{defaultInnerCutoff};     // offset 64
    f32 outerCutoff{defaultOuterCutoff};     // offset 68
    f32 _pad3[2]{};                          // offset 72 (array stride = 80)
};
// size = 80

// std140 size verification
static_assert(sizeof(AmbientLight)     == 32,  "AmbientLight std140 size mismatch");
static_assert(sizeof(DirectionalLight) == 32,  "DirectionalLight std140 size mismatch");
static_assert(sizeof(PointLight)       == 48,  "PointLight std140 size mismatch");
static_assert(sizeof(SpotLight)        == 80,  "SpotLight std140 size mismatch");

// ---- GPU-uploadable LightUBO (fixed-size arrays, memcpy-safe) ----
struct LightUBO {
    AmbientLight ambientLight{};
    DirectionalLight sunlight{};
    PointLight blobLights[1]{};
    SpotLight flashLights[1]{};
};

static_assert(sizeof(LightUBO) == 192, "LightUBO std140 size mismatch");

// ---- Light runtime class ----
class Light {
public:
    void init();
    void update();
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE const LightUBO& ubo() const {
        return m_ubo;
    }
    SV_FORCE_INLINE LightUBO& ubo() {
        return m_ubo;
    }

private:
    LightUBO m_ubo{};
};

} // namespace

using AmbientLight = light::AmbientLight;
using DirectionalLight = light::DirectionalLight;
using PointLight = light::PointLight;
using SpotLight = light::SpotLight;
using LightUBO = light::LightUBO;
using Light = light::Light;
