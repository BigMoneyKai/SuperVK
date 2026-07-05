#pragma once

#include "defines.h"
#include "texture.h"

#include <vulkan/vulkan.h>
#include <vector>

class TextureMan {
public:
    void init(
        const VkDevice& device,
        const VkPhysicalDevice& physicalDevice,
        const VkCommandPool& commandPool,
        const VkQueue& graphicsQueue
    );
    u32 loadTexture(const char* path);
    void destroy();

    SV_FORCE_INLINE const Texture& texture(u32 index) const {
        return m_textures[index];
    }
    SV_FORCE_INLINE u32 textureCount() const {
        return static_cast<u32>(m_textures.size());
    }

private:
    VkDevice m_device{VK_NULL_HANDLE};
    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
    VkCommandPool m_commandPool{VK_NULL_HANDLE};
    VkQueue m_graphicsQueue{VK_NULL_HANDLE};

    std::vector<Texture> m_textures;

};
