#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

class VulkanSampler : public VulkanObject
{
public:
    VulkanSampler(rad::Ref<VulkanDevice> device, const VkSamplerCreateInfo& createInfo);
    ~VulkanSampler();

    VkSampler GetHandle() const { return m_handle; }

private:
    rad::Ref<VulkanDevice> m_device;
    VkSampler m_handle = VK_NULL_HANDLE;

}; // class VulkanSampler

} // namespace rad
