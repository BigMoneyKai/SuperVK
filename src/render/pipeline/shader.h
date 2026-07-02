#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>
#include <vector>

enum ShaderType {
    ST_VERTEX_SHADER,
    ST_FRAGMENT_SHADER,
    ST_COMPUTE_SHADER,
    ST_GEOMETRY_SHADER,

    ST_MAX_NUM
};

struct ShaderDesc {

};

class Shader {
public:
    void init(const VkDevice& device, const char* path, ShaderType type);
    void load();
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE VkShaderStageFlagBits stage() const {
        return m_shaderStageCreateInfo.stage;
    }
    SV_FORCE_INLINE VkShaderModule module() const {
        return m_shaderModule;
    }

private:
    void readShader(const char* filePath);

private:
    const char* m_path{nullptr};
    ShaderType m_type;
    std::vector<u32> m_code;
    VkShaderModuleCreateInfo m_shaderModuleCreateInfo{};
    VkShaderModule m_shaderModule;
    VkPipelineShaderStageCreateInfo m_shaderStageCreateInfo{};
};
