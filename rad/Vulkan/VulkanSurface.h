#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

class VulkanSurface : public VulkanObject
{
public:
    VulkanSurface(rad::Ref<VulkanInstance> instance, VkSurfaceKHR handle);
    ~VulkanSurface();

    VkSurfaceKHR GetHandle() const { return m_handle; }

private:
    rad::Ref<VulkanInstance> m_instance;
    VkSurfaceKHR m_handle = VK_NULL_HANDLE;

}; // class VulkanSurface

} // namespace rad
