#pragma once

#include <vulkan/vulkan.h>

#include "defines.h"

#include <vector>

enum BufferType {
    BT_VERTEX_BUFFER,
    BT_INDEX_BUFFER,
    BT_STAGING_BUFFER,
    BT_UNIFORM_BUFFER,
    BT_STORAGE_BUFFER,

    BT_MAX_NUM,
};

class Buffer {
public:

    void init(
        BufferType type,
        const VkDevice& device,
        const VkPhysicalDevice& physicalDevice,
        void* data,
        u64 size
    );
    void destroy();
    void printDebugInfo();
    void update(const void* data, u64 size = 0);

    SV_FORCE_INLINE const VkBuffer& buffer() const {
        return m_buffer;
    }
    SV_FORCE_INLINE VkBuffer buffer() {
        return m_buffer;
    }
    SV_FORCE_INLINE u64 size() const {
        return m_size;
    }

private:
    void createBuffer();
    void allocateMemory(const VkPhysicalDevice& physicalDevice);

private:
    VkDevice m_device{VK_NULL_HANDLE};
    BufferType m_type{BT_MAX_NUM};
    VkMemoryRequirements m_memoryRequirements{};
    VkDeviceMemory m_memory{VK_NULL_HANDLE};
    VkMemoryAllocateInfo m_memoryAllocateInfo{};
    VkBufferCreateInfo m_bufferCreateInfo{};
    VkBuffer m_buffer{VK_NULL_HANDLE};

    void* m_data{nullptr};
    u64 m_size{0};
};
