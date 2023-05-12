#ifndef VULKAN_SEMAPHORE_H
#define VULKAN_SEMAPHORE_H

#include "VulkanCommon.h"

class VulkanSemaphore : public VulkanObject
{
public:
    VulkanSemaphore(rad::Ref<VulkanDevice> device, const VkSemaphoreCreateInfo& createInfo);
    VulkanSemaphore(rad::Ref<VulkanDevice> device, VkSemaphore handle);
    ~VulkanSemaphore();

    VkSemaphore GetHandle() const { return m_handle; }

private:
    rad::Ref<VulkanDevice> m_device;
    VkSemaphore m_handle = VK_NULL_HANDLE;
    bool m_isManaged = true;

}; // class VulkanSemaphore

#endif // VULKAN_SEMAPHORE_H
