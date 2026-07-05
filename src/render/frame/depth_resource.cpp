#include "depth_resource.h"
#include "debug/debugger.h"

void DepthResource::init(
    const VkDevice& device,
    const VkPhysicalDevice& physicalDevice,
    VkFormat depthFormat,
    VkExtent2D extent
) {
    m_device = device;
    // Create depth image
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = depthFormat;
    imageInfo.extent.width = extent.width;
    imageInfo.extent.height = extent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if(vkCreateImage(device, &imageInfo, nullptr, &m_depthImage) != VK_SUCCESS) {
        FATAL("Failed to create depth image");
    }

    // Allocate memory for depth image
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, m_depthImage, &memRequirements);

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    u32 memoryTypeIndex = UINT32_MAX;
    for(u32 i = 0; i < memProperties.memoryTypeCount; i++) {
        if((memRequirements.memoryTypeBits & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
            memoryTypeIndex = i;
            break;
        }
    }

    if(memoryTypeIndex == UINT32_MAX) {
        FATAL("Failed to find suitable memory type for depth image");
    }

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;

    if(vkAllocateMemory(device, &allocInfo, nullptr, &m_depthImageMemory) != VK_SUCCESS) {
        FATAL("Failed to allocate memory for depth image");
    }

    vkBindImageMemory(device, m_depthImage, m_depthImageMemory, 0);

    // Create depth image view
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_depthImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = depthFormat;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if(vkCreateImageView(device, &viewInfo, nullptr, &m_depthImageView) != VK_SUCCESS) {
        FATAL("Failed to create depth image view");
    }
}

void DepthResource::destroy() {
    if(m_depthImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(m_device, m_depthImageView, nullptr);
        m_depthImageView = VK_NULL_HANDLE;
    }
    if(m_depthImage != VK_NULL_HANDLE) {
        vkDestroyImage(m_device, m_depthImage, nullptr);
        m_depthImage = VK_NULL_HANDLE;
    }
    if(m_depthImageMemory != VK_NULL_HANDLE) {
        vkFreeMemory(m_device, m_depthImageMemory, nullptr);
        m_depthImageMemory = VK_NULL_HANDLE;
    }
}
