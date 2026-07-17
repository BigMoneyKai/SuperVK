#include "sampler.h"
#include "core/debug/debugger.h"

void Sampler::init(const VkDevice& device, const SamplerDesc& desc) {
    m_device = device;
    m_samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    m_samplerCreateInfo.magFilter = desc.magFilter;
    m_samplerCreateInfo.minFilter = desc.minFilter;
    m_samplerCreateInfo.addressModeU = desc.addressU;
    m_samplerCreateInfo.addressModeV = desc.addressV;
    m_samplerCreateInfo.addressModeW = desc.addressW;
    m_samplerCreateInfo.anisotropyEnable = desc.anisotropyEnable;
    m_samplerCreateInfo.maxAnisotropy = desc.maxAnisotropy;
    m_samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    m_samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
    m_samplerCreateInfo.compareEnable = VK_FALSE;
    m_samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    m_samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    m_samplerCreateInfo.mipLodBias = 0.0f;
    m_samplerCreateInfo.minLod = desc.minLod;
    m_samplerCreateInfo.maxLod = desc.maxLod;
    VK_CHECK_RESULT(vkCreateSampler(m_device, &m_samplerCreateInfo, nullptr, &m_sampler));
}

void Sampler::destroy() {
    vkDestroySampler(m_device, m_sampler, nullptr);
}
