#include "image.h"
#include "utils/utils.h"
#include "debug/debugger.h"
#include "render/base/command_pool.h"

void Image::init(
    const VkDevice& device,
    const VkPhysicalDevice& physicalDevice,
    const VkCommandPool& commandPool,
    const VkQueue& graphicsQueue,
    const ImageDesc& desc
) {
    m_device = device;
    m_commandPool = commandPool;
    m_graphicsQueue = graphicsQueue;
    m_format = desc.format;
    m_extent = {desc.width, desc.height, desc.depth};

    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = desc.imageType;
    imageCreateInfo.format = m_format;
    imageCreateInfo.extent = m_extent;
    imageCreateInfo.mipLevels = desc.mipLevels;
    imageCreateInfo.samples = desc.samples;
    imageCreateInfo.tiling = desc.tiling;
    imageCreateInfo.usage = desc.usage;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = desc.queueFamilyIndexCount;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VK_CHECK_RESULT(vkCreateImage(m_device, &imageCreateInfo, nullptr, &m_image));

    VkMemoryRequirements imageMemoryRequirements;
    vkGetImageMemoryRequirements(m_device, m_image, &imageMemoryRequirements);

    VkMemoryAllocateInfo imageMemoryAllocateInfo{};
    imageMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    imageMemoryAllocateInfo.allocationSize = imageMemoryRequirements.size;
    imageMemoryAllocateInfo.memoryTypeIndex = findMemoryType(
        physicalDevice,
        imageMemoryRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
    VK_CHECK_RESULT(vkAllocateMemory(m_device, &imageMemoryAllocateInfo, nullptr, &m_memory));

    VK_CHECK_RESULT(vkBindImageMemory(m_device, m_image, m_memory, 0));

    VkImageViewCreateInfo imageViewCreateInfo{};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.image = m_image;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = desc.format;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = desc.mipLevels;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;

    VK_CHECK_RESULT(vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &m_imageView));
}

void Image::transitionLayout(VkImageLayout newLayout) {
    VkCommandBuffer cmd =
        beginSingleTimeCommands(
            m_device,
            m_commandPool
        );

    VkImageMemoryBarrier barrier{};
    barrier.sType =
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

    barrier.oldLayout = m_layout;

    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex =
        VK_QUEUE_FAMILY_IGNORED;

    barrier.dstQueueFamilyIndex =
        VK_QUEUE_FAMILY_IGNORED;

    barrier.image = m_image;

    barrier.subresourceRange.aspectMask =
        VK_IMAGE_ASPECT_COLOR_BIT;

    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;

    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    barrier.srcAccessMask = 0;

    if (m_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
        newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    } else if (
        m_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
        newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    } else {
        FATAL("Unsupported image layout transition");
    }

    vkCmdPipelineBarrier(
        cmd,

        m_layout == VK_IMAGE_LAYOUT_UNDEFINED
            ? VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
            : VK_PIPELINE_STAGE_TRANSFER_BIT,
        newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            ? VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
            : VK_PIPELINE_STAGE_TRANSFER_BIT,

        0,

        0, nullptr,
        0, nullptr,

        1, &barrier
    );

    endSingleTimeCommands(
        m_device,
        m_graphicsQueue,
        m_commandPool,
        cmd
    );

    m_layout = newLayout;
}

void Image::destroy() {
    if(m_imageView != VK_NULL_HANDLE) {
        vkDestroyImageView(m_device, m_imageView, nullptr);
        m_imageView = VK_NULL_HANDLE;
    }
    if(m_image != VK_NULL_HANDLE) {
        vkDestroyImage(m_device, m_image, nullptr);
        m_image = VK_NULL_HANDLE;
    }
    if(m_memory != VK_NULL_HANDLE) {
        vkFreeMemory(m_device, m_memory, nullptr);
        m_memory = VK_NULL_HANDLE;
    }
}
