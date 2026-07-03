#pragma once

#include "defines.h"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace camera {
SV_INLINE constexpr glm::vec3 defaultEye = {0.f, 0.f, 2.f};
SV_INLINE constexpr glm::vec3 defaultCenter = {0.f, 0.f, 0.f};
SV_INLINE constexpr glm::vec3 defaultUp = {0.f, -1.f, 0.f};
SV_INLINE constexpr f32 defaultFov = 45.f;
SV_INLINE constexpr f32 defaultNear = 0.01f;
SV_INLINE constexpr f32 defaultFar = 1000.f;

SV_INLINE constexpr f32 defaultYaw = 0.f;
SV_INLINE constexpr f32 defaultPitch = 0.f;

SV_INLINE constexpr glm::vec2 defaultMouseDelta = {0.f, 0.f};
SV_INLINE constexpr f32 defaultScrollWheel      = 0.f;
SV_INLINE constexpr b32 defaultOrbitEnabled     = SV_FALSE;
SV_INLINE constexpr b32 defaultMoveForward      = SV_FALSE;
SV_INLINE constexpr b32 defaultMoveBackward     = SV_FALSE;
SV_INLINE constexpr b32 defaultMoveLeft         = SV_FALSE;
SV_INLINE constexpr b32 defaultMoveRight        = SV_FALSE;

struct CameraUBO {
    glm::mat4 view{};
    glm::mat4 proj{};
    glm::vec3 pos{};
};

// ---- Input bundle passed from App::update() to Camera::update() ----
struct CameraInput {
    glm::vec2 mouseDelta{defaultMouseDelta};
    f32 scrollWheel{defaultScrollWheel};
    b32 orbitEnabled{defaultOrbitEnabled};   // right mouse button held
    b32 moveForward{defaultMoveForward};    // W
    b32 moveBackward{defaultMoveBackward};   // S
    b32 moveLeft{defaultMoveLeft};       // A
    b32 moveRight{defaultMoveRight};      // D
};

class Camera {
public:
    void init(f32 aspect);
    void update(const CameraInput& input);
    void destroy();

    SV_FORCE_INLINE const CameraUBO& ubo() const {
        return m_ubo;
    }
    SV_FORCE_INLINE CameraUBO& ubo() {
        return m_ubo;
    }

private:
    CameraUBO m_ubo{};

    // orbital state
    glm::vec3 m_eye{defaultEye};
    glm::vec3 m_target{0.0f, 0.0f, 0.0f};
    f32 m_yaw{defaultYaw};
    f32 m_pitch{defaultPitch};
    f32 m_distance;
};

} // namespace

using CameraUBO = camera::CameraUBO;
using Camera = camera::Camera;
using CameraInput = camera::CameraInput;
