#include "scene/camera.h"
#include "glm/geometric.hpp"

#include <glm/gtc/matrix_transform.hpp>

// ---- orbit camera speed constants ----
static constexpr f32 kRotationSpeed = 0.0004f;
static constexpr f32 kZoomSpeed = 0.1f;
static constexpr f32 kWASDSpeed = 2.0f;
static constexpr f32 kUpDownSpeed = 0.001f;
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
  const f32 sinYaw = glm::sin(m_yaw);
  const f32 cosYaw = glm::cos(m_yaw);
  const f32 sinPitch = glm::sin(m_pitch);
  const f32 cosPitch = glm::cos(m_pitch);

  // ---- zoom (scroll wheel) ----
  m_distance -= input.scrollWheel * kZoomSpeed;
  m_distance = clampF(m_distance, kMinDistance, kMaxDistance);

  // ---- WASD pan (move target in view space) ----
  glm::vec3 forward{sinYaw, 0.0f, cosYaw};
  glm::vec3 right{forward.z, 0.0f, -forward.x};

  const f32 dt = input.deltaTime;
  if (input.moveForward) {
    m_target.x -= forward.x * kWASDSpeed * dt;
    m_target.z -= forward.z * kWASDSpeed * dt;
  }
  if (input.moveBackward) {
    m_target.x += forward.x * kWASDSpeed * dt;
    m_target.z += forward.z * kWASDSpeed * dt;
  }
  if (input.moveRight) {
    m_target.x -= right.x * kWASDSpeed * dt;
    m_target.z -= right.z * kWASDSpeed * dt;
  }
  if (input.moveLeft) {
    m_target.x += right.x * kWASDSpeed * dt;
    m_target.z += right.z * kWASDSpeed * dt;
  }
  if (input.moveUp)
    m_target.y -= defaultUp.y * kUpDownSpeed;
  if (input.moveDown)
    m_target.y += defaultUp.y * kUpDownSpeed;

  // ---- spherical → cartesian eye position ----
  glm::vec3 dir = {sinYaw * cosPitch, sinPitch, cosYaw * cosPitch};

  m_eye = m_target + dir * m_distance;

  // ---- update UBO ----
  m_ubo.view = glm::lookAt(m_eye, m_target, defaultUp);
  m_ubo.pos = m_eye;
  // m_ubo.proj is set once in init() — does not change unless aspect changes
}

void Camera::destroy() {
  m_ubo = {{}, {}, {}};
	m_eye = {defaultEye};
	m_target = {};
	m_yaw = defaultYaw;
	m_pitch = defaultPitch;
	m_distance = 0.f;
}
