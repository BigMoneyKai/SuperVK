#include "shader.h"
#include "core/debug/debugger.h"
#include "utils/utils.h"

#include <cstdlib>
#include <fstream>

void Shader::init(const VkDevice &device, const char *path, ShaderType type) {
  m_device = device;
  m_path = path;
  m_type = type;
  readShader(m_path);

  m_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  m_shaderModuleCreateInfo.codeSize =
      static_cast<size_t>(m_code.size() * sizeof(u32));
  m_shaderModuleCreateInfo.pCode = m_code.data();

  VK_CHECK_RESULT(vkCreateShaderModule(device, &m_shaderModuleCreateInfo,
                                       nullptr, &m_shaderModule));
  VkShaderStageFlagBits stage;
  switch (m_type) {
  case ST_VERTEX_SHADER:
    stage = VK_SHADER_STAGE_VERTEX_BIT;
    break;
  case ST_FRAGMENT_SHADER:
    stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    break;
  case ST_COMPUTE_SHADER:
    stage = VK_SHADER_STAGE_COMPUTE_BIT;
    break;
  case ST_GEOMETRY_SHADER:
    stage = VK_SHADER_STAGE_GEOMETRY_BIT;
    break;
  default:
    FATAL(LogCatag::Vulkan, "Unknown shader type");
    failure_exit();
  }
  m_shaderStageCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  m_shaderStageCreateInfo.stage = stage;
  m_shaderStageCreateInfo.module = m_shaderModule;
  m_shaderStageCreateInfo.pName = "main";
}

void Shader::load() {}

void Shader::destroy() {
  if (m_shaderModule != VK_NULL_HANDLE) {
    vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
    m_shaderModule = VK_NULL_HANDLE;
  }
}

void Shader::readShader(const char *path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    FATAL(LogCatag::Vulkan, "Failed to open shader file: {}", path);
  }

  const std::streamsize size = file.tellg();
  if (size <= 0) {
    FATAL(LogCatag::Vulkan, "Failed to read shader file size: {}", path);
  }
  if (size % 4 != 0) {
    FATAL(LogCatag::Vulkan,
          "The size of spir-v file should be aligned in 4 bytes");
  }

  m_code.resize(static_cast<size_t>(size) / sizeof(u32));

  file.seekg(0);
  file.read(reinterpret_cast<char *>(m_code.data()), size);
  if (!file) {
    FATAL(LogCatag::Vulkan, "Failed to read shader file: {}", path);
  }
}
