#include "command_pool.h"
#include "core/debug/debugger.h"

void CommandPool::init(const VkDevice &device, u32 graphicsQueueFamilyIndex) {
  m_device = device;
  commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  commandPoolCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
  VK_CHECK_RESULT(vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr,
                                      &commandPool));
}

void CommandPool::destroy() {
  vkDestroyCommandPool(m_device, commandPool, nullptr);
}
