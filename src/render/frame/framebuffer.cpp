#include "framebuffer.h"
#include "debug/debugger.h"

void Framebuffer::init(
    const VkDevice& device,
    const VkRenderPass& renderPass,
    const VkImageView& colorImageView,
    const VkImageView& depthImageView,
    VkExtent2D extent
) {
    m_device = device;
    std::vector<VkImageView> attachments = {colorImageView, depthImageView};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = attachments.size();
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    VK_CHECK_RESULT(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_framebuffer));

}

void Framebuffer::destroy() {
    if(m_framebuffer != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(m_device, m_framebuffer, nullptr);
        m_framebuffer = VK_NULL_HANDLE;
    }
}
