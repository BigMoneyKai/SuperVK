#include "buffer.h"
#include "core/debug/debugger.h"
#include "utils/utils.h"

#include <cstring>
#include <vector>

static const char *bufferTypes[6] = {
    "vertex buffer",  "index buffer",   "staging buffer",
    "uniform buffer", "storage buffer",
};

void Buffer::init(BufferType type, const VkDevice &device,
                  const VkPhysicalDevice &physicalDevice, void *data,
                  u64 size) {
  m_device = device;
  m_type = type;
  m_data = data;
  if (m_data == nullptr) {
    WARNING(LogCatag::Vulkan, "No data in this address");
  }
  m_size = size;
  createBuffer();
  allocateMemory(physicalDevice);
}

void Buffer::destroy() {
  if (m_buffer != VK_NULL_HANDLE) {
    vkDestroyBuffer(m_device, m_buffer, nullptr);
    m_buffer = VK_NULL_HANDLE;
  }
  if (m_memory != VK_NULL_HANDLE) {
    vkFreeMemory(m_device, m_memory, nullptr);
    m_memory = VK_NULL_HANDLE;
  }
}

void Buffer::createBuffer() {
  VkBufferUsageFlags usage;
  switch (m_type) {
  case BT_VERTEX_BUFFER:
    usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    break;
  case BT_INDEX_BUFFER:
    usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    break;
  case BT_STAGING_BUFFER:
    usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    break;
  case BT_UNIFORM_BUFFER:
    usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    break;
  case BT_STORAGE_BUFFER:
    usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    break;
  case BT_MAX_NUM:
    break;
  }
  m_bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  m_bufferCreateInfo.size = m_size;
  m_bufferCreateInfo.usage = usage;
  m_bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  VK_CHECK_RESULT(
      vkCreateBuffer(m_device, &m_bufferCreateInfo, nullptr, &m_buffer));
}

void Buffer::allocateMemory(const VkPhysicalDevice &physicalDevice) {
  vkGetBufferMemoryRequirements(m_device, m_buffer, &m_memoryRequirements);

  m_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  m_memoryAllocateInfo.allocationSize = m_memoryRequirements.size;
  switch (m_type) {
  case BT_VERTEX_BUFFER:
  case BT_INDEX_BUFFER:
  case BT_STAGING_BUFFER:
    m_memoryAllocateInfo.memoryTypeIndex =
        findMemoryType(physicalDevice, m_memoryRequirements.memoryTypeBits,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    break;
  case BT_UNIFORM_BUFFER:
    m_memoryAllocateInfo.memoryTypeIndex =
        findMemoryType(physicalDevice, m_memoryRequirements.memoryTypeBits,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    break;
  case BT_STORAGE_BUFFER:
    m_memoryAllocateInfo.memoryTypeIndex =
        findMemoryType(physicalDevice, m_memoryRequirements.memoryTypeBits,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    break;
  case BT_MAX_NUM:
  default:
    break;
  }
  VK_CHECK_RESULT(
      vkAllocateMemory(m_device, &m_memoryAllocateInfo, nullptr, &m_memory));
  void *bufferData;
  if (m_data != nullptr && m_bufferCreateInfo.size > 0) {
    VK_CHECK_RESULT(vkMapMemory(m_device, m_memory, 0, m_bufferCreateInfo.size,
                                0, &bufferData));
    memcpy(bufferData, m_data, m_bufferCreateInfo.size);
    vkUnmapMemory(m_device, m_memory);
  }
  VK_CHECK_RESULT(vkBindBufferMemory(m_device, m_buffer, m_memory, 0));
}

void Buffer::printDebugInfo() {
  DEBUG(LogCatag::Render, "Buffer type: {}", bufferTypes[m_type]);
  DEBUG(LogCatag::Render, "Buffer Size = {}", m_size);
  DEBUG(LogCatag::Render, "Usage = {}", m_bufferCreateInfo.usage);
  DEBUG(LogCatag::Render, "MemorySize = {} ",
        m_memoryAllocateInfo.allocationSize);
}

void Buffer::update(const void *data, u64 size) {
  if (m_memory == VK_NULL_HANDLE || data == nullptr) {
    return;
  }

  const u64 copySize = size == 0 ? m_size : size;
  if (copySize == 0) {
    return;
  }

  void *bufferData = nullptr;
  VK_CHECK_RESULT(vkMapMemory(m_device, m_memory, 0, copySize, 0, &bufferData));
  memcpy(bufferData, data, static_cast<size_t>(copySize));
  vkUnmapMemory(m_device, m_memory);
}
