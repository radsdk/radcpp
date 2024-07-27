#include <rad/Vulkan/VulkanPhysicalDevice.h>
#include <rad/Vulkan/VulkanInstance.h>
#include <rad/Vulkan/VulkanDevice.h>

namespace rad
{

VulkanPhysicalDevice::VulkanPhysicalDevice(Ref<VulkanInstance> instance, VkPhysicalDevice handle) :
    m_instance(std::move(instance)),
    m_handle(handle)
{
    m_properties = {};
    vkGetPhysicalDeviceProperties(m_handle, &m_properties);
    if (IsVersionMatchOrGreater(1, 1, 0))
    {
        m_properties2 = {};
        m_properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
        VK_STRUCTURE_CHAIN_BEGIN(m_properties2);
        if (IsVersionMatchOrGreater(1, 2, 0))
        {
            m_vk11Properties = {};
            m_vk11Properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES;
            VK_STRUCTURE_CHAIN_ADD(m_properties2, m_vk11Properties);
        }
        if (IsVersionMatchOrGreater(1, 2, 0))
        {
            m_vk12Properties = {};
            m_vk12Properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES;
            VK_STRUCTURE_CHAIN_ADD(m_properties2, m_vk12Properties);
        }
        if (IsVersionMatchOrGreater(1, 3, 0))
        {
            m_vk13Properties = {};
            m_vk13Properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES;
            VK_STRUCTURE_CHAIN_ADD(m_properties2, m_vk13Properties);
        }
        VK_STRUCTURE_CHAIN_END(m_properties2);
        vkGetPhysicalDeviceProperties2(m_handle, &m_properties2);
    }

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &queueFamilyCount, nullptr);
    if (queueFamilyCount > 0)
    {
        m_queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            m_handle, &queueFamilyCount, m_queueFamilies.data());
    }

    vkGetPhysicalDeviceMemoryProperties(m_handle, &m_memoryProperties);

    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, nullptr);
    if (extensionCount > 0)
    {
        m_extensions.resize(extensionCount);
        vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, m_extensions.data());
    }

    m_features = {};
    vkGetPhysicalDeviceFeatures(m_handle, &m_features);
    if (IsVersionMatchOrGreater(1, 1, 0))
    {
        m_features2 = {};
        m_features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        VK_STRUCTURE_CHAIN_BEGIN(m_features2);
        if (IsVersionMatchOrGreater(1, 2, 0))
        {
            m_vk11Features = {};
            m_vk11Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
            VK_STRUCTURE_CHAIN_ADD(m_features2, m_vk11Features);
        }
        if (IsVersionMatchOrGreater(1, 2, 0))
        {
            m_vk12Features = {};
            m_vk12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
            VK_STRUCTURE_CHAIN_ADD(m_features2, m_vk12Features);
        }
        if (IsVersionMatchOrGreater(1, 3, 0))
        {
            m_vk13Features = {};
            m_vk13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
            VK_STRUCTURE_CHAIN_ADD(m_features2, m_vk13Features);
        }
        VK_STRUCTURE_CHAIN_END(m_features2);
        vkGetPhysicalDeviceFeatures2(m_handle, &m_features2);
    }
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
}

bool VulkanPhysicalDevice::IsVersionMatchOrGreater(
    uint32_t major, uint32_t minor, uint32_t patch)
{
    return vkIsVersionMatchOrGreater(m_properties.apiVersion, major, minor, patch);
}

bool VulkanPhysicalDevice::IsVendorAMD() const
{
    return (m_properties.vendorID == VulkanVendorAMD1) ||
        (m_properties.vendorID == VulkanVendorAMD2);
}

bool VulkanPhysicalDevice::IsVendorNVIDIA() const
{
    return (m_properties.vendorID == VulkanVendorNVIDIA);
}

bool VulkanPhysicalDevice::IsVendorIntel() const
{
    return (m_properties.vendorID == VulkanVendorIntel);
}

bool VulkanPhysicalDevice::IsIntegrated() const
{
    return (m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
}

bool VulkanPhysicalDevice::IsDiscrete() const
{
    return (m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
}

bool VulkanPhysicalDevice::IsVirtual() const
{
    return (m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU);
}

bool VulkanPhysicalDevice::IsCPU() const
{
    return (m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU);
}

bool VulkanPhysicalDevice::IsExtensionSupported(std::string_view name) const
{
    for (const VkExtensionProperties& extension : m_extensions)
    {
        if (extension.extensionName == name)
        {
            return true;
        }
    }
    return false;
}

VkFormatProperties VulkanPhysicalDevice::GetFormatProperties(VkFormat format) const
{
    VkFormatProperties formatProperties = {};
    vkGetPhysicalDeviceFormatProperties(m_handle, format, &formatProperties);
    return formatProperties;
}

bool VulkanPhysicalDevice::IsFormatSupported(
    VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    VkFormatProperties properties = GetFormatProperties(format);
    if ((tiling == VK_IMAGE_TILING_LINEAR) &&
        HasBits(properties.linearTilingFeatures, features))
    {
        return true;
    }
    else if ((tiling == VK_IMAGE_TILING_OPTIMAL) &&
        HasBits(properties.optimalTilingFeatures, features))
    {
        return true;
    }
    else
    {
        return false;
    }
}

VkSurfaceCapabilitiesKHR VulkanPhysicalDevice::GetSurfaceCapabilities(VkSurfaceKHR surface)
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        m_handle, surface, &surfaceCapabilities));
    return surfaceCapabilities;
}

std::vector<VkPresentModeKHR> VulkanPhysicalDevice::GetSurfacePresentModes(VkSurfaceKHR surface)
{
    std::vector<VkPresentModeKHR> presentModes;
    uint32_t presentModeCount = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
        m_handle, surface, &presentModeCount, nullptr));
    if (presentModeCount > 0)
    {
        presentModes.resize(presentModeCount);
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
            m_handle, surface, &presentModeCount, presentModes.data()));
    }
    return presentModes;
}

std::vector<VkSurfaceFormatKHR> VulkanPhysicalDevice::GetSurfaceFormats(VkSurfaceKHR surface)
{
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    uint32_t formatCount = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(
        m_handle, surface, &formatCount, nullptr));
    if (formatCount > 0)
    {
        surfaceFormats.resize(formatCount);
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(
            m_handle, surface, &formatCount, surfaceFormats.data()));
    }
    return surfaceFormats;
}

rad::Ref<VulkanDevice> VulkanPhysicalDevice::CreateDevice(
    const std::set<std::string>& extensionNames)
{
    return RAD_NEW VulkanDevice(m_instance, this, extensionNames);
}

} // namespace rad
