#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

class VulkanEvent : public VulkanObject
{
public:
    VulkanEvent(rad::Ref<VulkanDevice> device, const VkEventCreateInfo& createInfo);
    ~VulkanEvent();

    VkEvent GetHandle() const { return m_handle; }

private:
    rad::Ref<VulkanDevice> m_device;
    VkEvent m_handle = VK_NULL_HANDLE;

}; // class VulkanEvent

} // namespace rad
