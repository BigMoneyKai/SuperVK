#include "scene/camera.h"
#include "glm/geometric.hpp"

#include <glm/gtc/matrix_transform.hpp>

// ---- orbit camera speed constants ----
static constexpr f32 kRotationSpeed = 0.0005f;
static constexpr f32 kZoomSpeed = 0.1f;
static constexpr f32 kMoveSpeed = 0.01f;
static constexpr f32 kMinDistance = 0.5f;
static constexpr f32 kMaxDistance = 50.0f;
static constexpr f32 kMaxPitch = 1.55f; // ~89 degrees in radians

// simple clamp helper (avoids glm header dependency for glm::clamp)
static SV_FORCE_INLINE f32 clampF(f32 v, f32 lo, f32 hi) {
  return v < lo ? lo : (v > hi ? hi : v);
}

void Camera::init(f32 aspect) {
  m_eye = defaultEye;
  m_target = glm::vec3(0.0f, 0.0f, 0.0f);
  m_yaw = 0.0f;
  m_pitch = 0.0f;
  m_distance = glm::distance(m_eye, m_target); // = 2.0f initially

  m_ubo.view = glm::lookAt(m_eye, m_target, defaultUp);

  m_ubo.proj = glm::perspective(glm::radians(defaultFov), aspect, defaultNear,
                                defaultFar);

  m_ubo.pos = m_eye;
}

void Camera::update(const CameraInput &input) {
  m_yaw += input.mouseDelta.x * kRotationSpeed;
  m_pitch += input.mouseDelta.y * kRotationSpeed;
  m_pitch = clampF(m_pitch, -kMaxPitch, kMaxPitch);

  // ---- zoom (scroll wheel) ----
  m_distance -= input.scrollWheel * kZoomSpeed;
  m_distance = clampF(m_distance, kMinDistance, kMaxDistance);

  // ---- WASD pan (move target in view space) ----
  glm::vec3 forward = m_target - m_eye;
	forward.y = 0.f;
	forward = glm::normalize(forward);
  glm::vec3 right = glm::normalize(glm::cross(forward, defaultUp));
  glm::vec3 up = glm::normalize(glm::cross(right, forward));

  if (input.moveForward) {
    m_target.x += forward.x * kMoveSpeed;
    m_target.z += forward.z * kMoveSpeed;
  }
  if (input.moveBackward) {
    m_target.x -= forward.x * kMoveSpeed;
    m_target.z -= forward.z * kMoveSpeed;
  }
	  if (input.moveRight) {
    m_target.x += right.x * kMoveSpeed;
    m_target.z += right.z * kMoveSpeed;
  }
  if (input.moveLeft) {
    m_target.x -= right.x * kMoveSpeed;
    m_target.z -= right.z * kMoveSpeed;
  }
  if (input.moveUp)
    m_target.y -= up.y * kMoveSpeed;
  if (input.moveDown)
    m_target.y += up.y * kMoveSpeed;

  // ---- spherical → cartesian eye position ----
  glm::vec3 dir;
  dir.x = glm::sin(m_yaw) * glm::cos(m_pitch);
  dir.y = glm::sin(m_pitch);
  dir.z = glm::cos(m_yaw) * glm::cos(m_pitch);

  m_eye = m_target + dir * m_distance;

  // ---- update UBO ----
  m_ubo.view = glm::lookAt(m_eye, m_target, defaultUp);
  m_ubo.pos = m_eye;
  // m_ubo.proj is set once in init() — does not change unless aspect changes
}

void Camera::destroy() {
  // no Vulkan resources owned by Camera
}
