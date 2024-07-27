#include <rad/Vulkan/VulkanFramebuffer.h>
#include <rad/Vulkan/VulkanDevice.h>

namespace rad
{

VulkanFramebuffer::VulkanFramebuffer(
    rad::Ref<VulkanDevice> device, const VkFramebufferCreateInfo& createInfo) :
    m_device(device)
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateFramebuffer(m_device->GetHandle(), &createInfo, nullptr, &m_handle));
    m_attachmentCount = createInfo.attachmentCount;
    m_width = createInfo.width;
    m_height = createInfo.height;
    m_layers = createInfo.layers;
}

VulkanFramebuffer::~VulkanFramebuffer()
{
    m_device->GetFunctionTable()->
        vkDestroyFramebuffer(m_device->GetHandle(), m_handle, nullptr);
}

} // namespace rad
