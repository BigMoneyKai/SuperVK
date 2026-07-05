#include "texture.h"
#include "debug/debugger.h"
#include "utils/utils.h"

void Texture::init(
    const VkDevice& device,
    const VkPhysicalDevice& physicalDevice,
    const VkCommandPool& commandPool,
    const VkQueue& graphicsQueue,
    const char* path
) {
    m_device = device;
    m_physicalDevice = physicalDevice;
    m_commandPool = commandPool;
    m_graphicsQueue = graphicsQueue;

    i32 texWidth, texHeight, texChannels;
    m_pixels = stbi_load(
        path,
        &texWidth,
        &texHeight,
        &texChannels,
        STBI_rgb_alpha
    );
    if(!m_pixels) {
        FATAL("Failed to load texture image");
    }
    u64 pixelDataSize = static_cast<u64>(texWidth) * static_cast<u64>(texHeight) * 4; // RGBA
    m_stagingBuffer.init(BT_STAGING_BUFFER, m_device, m_physicalDevice, m_pixels, pixelDataSize);
    ImageDesc imageDesc{};
    imageDesc.width = static_cast<u32>(texWidth);
    imageDesc.height = static_cast<u32>(texHeight);
    m_image.init(m_device, m_physicalDevice, m_commandPool, m_graphicsQueue, imageDesc);

    m_image.transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage({imageDesc.width, imageDesc.height, imageDesc.depth});
    m_image.transitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    m_stagingBuffer.destroy();
    stbi_image_free(m_pixels);

    m_sampler.init(m_device, {});
}

void Texture::destroy() {
    m_image.destroy();
    m_sampler.destroy();
}

void Texture::copyBufferToImage(const VkExtent3D& extent) {
    VkCommandBuffer cmd = beginSingleTimeCommands(m_device, m_commandPool);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = extent;

    vkCmdCopyBufferToImage(
        cmd,
        m_stagingBuffer.buffer(),
        m_image.image(),
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    endSingleTimeCommands(
        m_device,
        m_graphicsQueue,
        m_commandPool,
        cmd
    );
}
