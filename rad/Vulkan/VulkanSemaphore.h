#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

class VulkanSemaphore : public VulkanObject
{
public:
    VulkanSemaphore(rad::Ref<VulkanDevice> device, const VkSemaphoreCreateInfo& createInfo);
    ~VulkanSemaphore();

    VkSemaphore GetHandle() const { return m_handle; }

private:
    rad::Ref<VulkanDevice> m_device;
    VkSemaphore m_handle = VK_NULL_HANDLE;

}; // class VulkanSemaphore

} // namespace rad
