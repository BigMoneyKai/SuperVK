#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"
#include <vulkan/vulkan.h>

namespace sampler {

constexpr VkFilter defaultMagFilter = VK_FILTER_LINEAR;
constexpr VkFilter defaultMinFilter = VK_FILTER_LINEAR;
constexpr VkSamplerAddressMode defaultAddressU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
constexpr VkSamplerAddressMode defaultAddressV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
constexpr VkSamplerAddressMode defaultAddressW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
constexpr b32 defaultAnisotropyEnable = VK_FALSE;
constexpr f32 defaultMaxAnisotropy = 1.0f;
constexpr f32 defaultMinLod = 0.0f;
constexpr f32 defaultMaxLod = 0.0f;

struct SamplerDesc {
  VkFilter magFilter{defaultMagFilter};
  VkFilter minFilter{defaultMinFilter};
  VkSamplerAddressMode addressU{defaultAddressU};
  VkSamplerAddressMode addressV{defaultAddressV};
  VkSamplerAddressMode addressW{defaultAddressW};
  b32 anisotropyEnable{defaultAnisotropyEnable};
  f32 maxAnisotropy{defaultMaxAnisotropy};
  f32 minLod{defaultMinLod};
  f32 maxLod{defaultMaxLod};
};

class Sampler {
public:
  void init(const VkDevice &device, const SamplerDesc &desc);
  void destroy();

  SV_FORCE_INLINE const VkSampler &sampler() const { return m_sampler; }
  SV_FORCE_INLINE VkSampler &sampler() { return m_sampler; }

private:
  VkDevice m_device{VK_NULL_HANDLE};
  VkSamplerCreateInfo m_samplerCreateInfo{};
  VkSampler m_sampler{VK_NULL_HANDLE};
};

} // namespace sampler

using Sampler = sampler::Sampler;
using SamplerDesc = sampler::SamplerDesc;
