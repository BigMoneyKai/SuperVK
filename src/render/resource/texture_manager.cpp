#include "texture_manager.h"

void TextureMan::init(
    const VkDevice& device,
    const VkPhysicalDevice& physicalDevice,
    const VkCommandPool& commandPool,
    const VkQueue& graphicsQueue
) {
    m_device = device;
    m_physicalDevice = physicalDevice;
    m_commandPool = commandPool;
    m_graphicsQueue = graphicsQueue;

}

u32 TextureMan::loadTexture(const char* path) {
    Texture texture{};
    texture.init(m_device, m_physicalDevice, m_commandPool, m_graphicsQueue, path);
    m_textures.push_back(std::move(texture));

    return static_cast<u32>(m_textures.size() - 1);
}

void TextureMan::destroy() {
    for(auto texture : m_textures) {
        texture.destroy();
    }
}
