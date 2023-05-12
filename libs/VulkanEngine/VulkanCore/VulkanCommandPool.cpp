#include "VulkanCommandPool.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"

VulkanCommandPool::VulkanCommandPool(rad::Ref<VulkanDevice> device, const VkCommandPoolCreateInfo& createInfo) :
    m_device(std::move(device))
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateCommandPool(m_device->GetHandle(), &createInfo, nullptr, &m_handle));
}

VulkanCommandPool::~VulkanCommandPool()
{
    m_device->GetFunctionTable()->
        vkDestroyCommandPool(m_device->GetHandle(), m_handle, nullptr);
    m_handle = VK_NULL_HANDLE;
}

rad::Ref<VulkanCommandBuffer> VulkanCommandPool::Allocate(VkCommandBufferLevel level)
{
    return new VulkanCommandBuffer(m_device, this, level);
}

void VulkanCommandPool::Trim()
{
    m_device->GetFunctionTable()->
        vkTrimCommandPool(m_device->GetHandle(), m_handle, 0);
}

void VulkanCommandPool::Reset(VkCommandPoolResetFlags flags)
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkResetCommandPool(m_device->GetHandle(), m_handle, flags));
}
