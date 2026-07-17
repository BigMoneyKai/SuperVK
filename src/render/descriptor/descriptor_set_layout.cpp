#include "descriptor_set_layout.h"
#include "core/debug/debugger.h"

void DescriptorSetLayout::init(const VkDevice& device, const std::vector<VkDescriptorSetLayoutBinding>& bindings) {
    m_device = device;
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<u32>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_layout));
}

void DescriptorSetLayout::destroy() {
    if(m_layout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
        m_layout = VK_NULL_HANDLE;
    }
}
