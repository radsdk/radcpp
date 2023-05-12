#ifndef VULKAN_INSTANCE_H
#define VULKAN_INSTANCE_H

#include "VulkanCommon.h"
#include <set>

class VulkanInstance : public VulkanObject
{
public:
    VulkanInstance();
    ~VulkanInstance();

    void EnableValidationLayer(bool enable = true) { m_enableValidationLayer = enable; }
    bool Init(std::string_view appName, uint32_t appVersion,
        std::set<std::string> extensionRequested);

    VkInstance GetHandle() const { return m_handle; }
    uint32_t GetApiVersion() const { return m_apiVersion; }

    bool SupportsExtension(std::string_view extensionName);

    std::vector<rad::Ref<VulkanPhysicalDevice>> EnumeratePhysicalDevices();

private:
    static std::vector<VkLayerProperties> EnumerateInstanceLayers();
    static std::vector<VkExtensionProperties> EnumerateInstanceExtensions(const char* layerName);

    VkInstance m_handle = VK_NULL_HANDLE;
    uint32_t m_apiVersion = 0;
    std::set<std::string> m_enabledExtensions;
    bool m_enableValidationLayer = false;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

}; // class VulkanInstance

#endif // VULKAN_INSTANCE_H
