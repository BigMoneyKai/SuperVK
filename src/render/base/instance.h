#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"

#include <vector>
#include <vulkan/vulkan.h>

class Instance {
public:
  void init();
  void destroy();

  SV_FORCE_INLINE const VkInstance &instance() const { return m_instance; }
  SV_FORCE_INLINE VkInstance instance() { return m_instance; }
  SV_FORCE_INLINE const char *const *extensions() const {
    return m_extensions.data();
  };

private:
  b32 enable_validation_layer();
  void enable_extensions();

private:
  VkApplicationInfo m_appInfo{};
  VkInstance m_instance;
  std::vector<const char *> m_validationLayers{"VK_LAYER_KHRONOS_validation"};
  std::vector<const char *> m_extensions;
};
