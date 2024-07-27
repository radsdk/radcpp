#include <rad/Vulkan/VulkanSemaphore.h>
#include <rad/Vulkan/VulkanDevice.h>

namespace rad
{

VulkanSemaphore::VulkanSemaphore(
    rad::Ref<VulkanDevice> device, const VkSemaphoreCreateInfo& createInfo) :
    m_device(std::move(device))
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateSemaphore(m_device->GetHandle(), &createInfo, nullptr, &m_handle));
}

VulkanSemaphore::~VulkanSemaphore()
{
    if (m_handle != VK_NULL_HANDLE)
    {
        m_device->GetFunctionTable()->
            vkDestroySemaphore(m_device->GetHandle(), m_handle, nullptr);
        m_handle = VK_NULL_HANDLE;
    }
}

} // namespace rad
