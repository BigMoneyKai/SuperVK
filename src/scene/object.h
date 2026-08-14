#pragma once

#include "core/define/compiler.h"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace object {

SV_INLINE constexpr glm::mat4 defaultModel = glm::mat4(1.0f);

struct ObjectUBO {
  glm::mat4 model{defaultModel};
};

class Object {
public:
  void init();
  void update();
  void destroy();

  SV_FORCE_INLINE const ObjectUBO &object() const { return m_ubo; }
  SV_FORCE_INLINE ObjectUBO &object() { return m_ubo; }

  SV_FORCE_INLINE const ObjectUBO &ubo() const { return m_ubo; }
  SV_FORCE_INLINE ObjectUBO &ubo() { return m_ubo; }

private:
  ObjectUBO m_ubo{};
};

} // namespace object

using ObjectUBO = object::ObjectUBO;
using Object = object::Object;
