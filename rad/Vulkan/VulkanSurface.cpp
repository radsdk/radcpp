#include <rad/Vulkan/VulkanSurface.h>
#include <rad/Vulkan/VulkanInstance.h>

namespace rad
{

VulkanSurface::VulkanSurface(rad::Ref<VulkanInstance> instance, VkSurfaceKHR handle) :
    m_instance(std::move(instance)),
    m_handle(handle)
{
}

VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(m_instance->GetHandle(), m_handle, nullptr);
}

} // namespace rad
