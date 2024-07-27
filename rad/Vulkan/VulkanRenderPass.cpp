#include <rad/Vulkan/VulkanRenderPass.h>
#include <rad/Vulkan/VulkanDevice.h>

namespace rad
{

VulkanRenderPass::VulkanRenderPass(
    rad::Ref<VulkanDevice> device,
    const VkRenderPassCreateInfo& createInfo) :
    m_device(std::move(device))
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateRenderPass(m_device->GetHandle(), &createInfo, nullptr, &m_handle));
}

VulkanRenderPass::~VulkanRenderPass()
{
    m_device->GetFunctionTable()->
        vkDestroyRenderPass(m_device->GetHandle(), m_handle, nullptr);
    m_handle = VK_NULL_HANDLE;
}

} // namespace rad
