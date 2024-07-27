#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

class VulkanInstance : public VulkanObject
{
public:
    VulkanInstance();
    ~VulkanInstance();

    VkInstance GetHandle() { return m_handle; }
    uint32_t GetVersion() const { return m_version; }
    bool IsVersionMatchOrGreater(uint32_t major, uint32_t minor, uint32_t patch);

    bool Init(std::string_view appName, uint32_t appVersion);
    bool IsExtensionSupported(std::string_view name) const;
    std::vector<Ref<VulkanPhysicalDevice>> EnumeratePhysicalDevices();

private:
    VkInstance m_handle = VK_NULL_HANDLE;
    uint32_t m_version = 0;
    std::set<std::string, StringLess> m_enabledExtensions;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

}; // class VulkanInstance

} // namespace rad
