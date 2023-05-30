#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"

VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    switch (severity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:  LogVulkan(Verbose, "[%s] %s",
        pCallbackData->pMessageIdName, pCallbackData->pMessage); break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:     LogVulkan(Info, "[%s] %s",
        pCallbackData->pMessageIdName, pCallbackData->pMessage); break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:  LogVulkan(Warn, "[%s] %s",
        pCallbackData->pMessageIdName, pCallbackData->pMessage); break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:    LogVulkan(Error, "[%s] %s",
        pCallbackData->pMessageIdName, pCallbackData->pMessage); break;
    }

    return VK_FALSE;
}

VulkanInstance::VulkanInstance()
{
}

VulkanInstance::~VulkanInstance()
{
    if (m_debugMessenger != VK_NULL_HANDLE)
    {
        vkDestroyDebugUtilsMessengerEXT(m_handle, m_debugMessenger, nullptr);
        m_debugMessenger = VK_NULL_HANDLE;
    }

    vkDestroyInstance(m_handle, nullptr);
}

bool HasExtension(rad::Span<VkExtensionProperties> extensions, std::string_view extensionName)
{
    for (const VkExtensionProperties& extension : extensions)
    {
        if (rad::StrEqual(extension.extensionName, extensionName))
        {
            return true;
        }
    }
    return false;
}

bool VulkanInstance::Init(std::string_view appName, uint32_t appVersion, std::set<std::string> extensionRequested)
{
    VK_CHECK(volkInitialize());

    if (vkEnumerateInstanceVersion)
    {
        vkEnumerateInstanceVersion(&m_apiVersion);
        LogVulkan(Info, "Vulkan instance version: %s", GetVulkanVersionString(m_apiVersion).c_str());
    }

    std::vector<const char*> enabledLayerNames;

    std::vector<VkLayerProperties> layers = EnumerateInstanceLayers();
    if (m_enableValidationLayer)
    {
        const char* pValidationLayerName = "VK_LAYER_KHRONOS_validation";
        bool validationLayerFound = false;
        for (const VkLayerProperties& layer : layers)
        {
            if (rad::StrEqual(layer.layerName, pValidationLayerName))
            {
                enabledLayerNames.push_back(pValidationLayerName);
                validationLayerFound = true;
            }
        }

        if (!validationLayerFound)
        {
            LogVulkan(Warn, "Failed to find validation layer %s", pValidationLayerName);
            m_enableValidationLayer = false;
        }
    }

    std::vector<VkExtensionProperties> extensionProps = EnumerateInstanceExtensions(nullptr);
    m_enabledExtensions = { extensionRequested.begin(), extensionRequested.end() };

    if (HasExtension(extensionProps, "VK_KHR_get_physical_device_properties2"))
    {
        m_enabledExtensions.insert("VK_KHR_get_physical_device_properties2");
    }
    if (HasExtension(extensionProps, "VK_KHR_get_surface_capabilities2"))
    {
        m_enabledExtensions.insert("VK_KHR_get_surface_capabilities2");
    }

    if (m_enableValidationLayer)
    {
        if (HasExtension(extensionProps, VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
        {
            m_enabledExtensions.insert(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        else
        {
            LogVulkan(Warn, "Cannot enable validation layer: extension not supported: %s",
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            m_enableValidationLayer = false;
        }
    }

    std::vector<const char*> enabledExtensionNames;
    for (const std::string& extension : m_enabledExtensions)
    {
        enabledExtensionNames.push_back(extension.data());
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = appName.data();
    appInfo.applicationVersion = appVersion;
    appInfo.pEngineName = appName.data();
    appInfo.engineVersion = appVersion;
    appInfo.apiVersion = m_apiVersion;

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayerNames.size());
    instanceCreateInfo.ppEnabledLayerNames = enabledLayerNames.data();
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensionNames.size());
    instanceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames.data();

    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
    if (m_enableValidationLayer)
    {
        debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugMessengerCreateInfo.pNext = nullptr;
        debugMessengerCreateInfo.flags = 0;
        debugMessengerCreateInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugMessengerCreateInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugMessengerCreateInfo.pfnUserCallback = DebugUtilsMessengerCallback;
        debugMessengerCreateInfo.pUserData = nullptr;

        AppendVulkanStructureChain(instanceCreateInfo, debugMessengerCreateInfo);
    }

    VK_CHECK(vkCreateInstance(&instanceCreateInfo, nullptr, &m_handle));
    if (m_handle)
    {
        volkLoadInstance(m_handle);
    }

    if (m_enableValidationLayer)
    {
        VK_CHECK(vkCreateDebugUtilsMessengerEXT(m_handle, &debugMessengerCreateInfo, nullptr, &m_debugMessenger));
    }

    return true;
}

bool VulkanInstance::SupportsExtension(std::string_view extensionName)
{
    return m_enabledExtensions.contains(extensionName.data());
}

std::vector<rad::Ref<VulkanPhysicalDevice>> VulkanInstance::EnumeratePhysicalDevices()
{
    std::vector<rad::Ref<VulkanPhysicalDevice>> physicalDevices;
    uint32_t physicalDeviceCount = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(GetHandle(), &physicalDeviceCount, nullptr));
    if (physicalDeviceCount > 0)
    {
        std::vector<VkPhysicalDevice> physicalDevicesHandles(physicalDeviceCount);
        VK_CHECK(vkEnumeratePhysicalDevices(GetHandle(), &physicalDeviceCount, physicalDevicesHandles.data()));
        physicalDevices.resize(physicalDevicesHandles.size());
        for (int i = 0; i < physicalDevicesHandles.size(); i++)
        {
            physicalDevices[i] = new VulkanPhysicalDevice(this, physicalDevicesHandles[i]);
        }
    }
    return physicalDevices;
}

std::vector<VkLayerProperties> VulkanInstance::EnumerateInstanceLayers()
{
    // It's possible, though very rare, that the number of instance layers could change.
    std::vector<VkLayerProperties> layers;
    uint32_t layerCount = 0;
    VkResult result = VK_SUCCESS;
    do {
        VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));
        if (layerCount > 0)
        {
            layers.resize(layerCount);
            result = vkEnumerateInstanceLayerProperties(&layerCount, layers.data());
        }
    } while (result == VK_INCOMPLETE);

    if (result < 0)
    {
        LogVulkan(Warn, "vkEnumerateInstanceLayerProperties failed with %s",
            string_VkResult(result));
    }

    return layers;
}

std::vector<VkExtensionProperties> VulkanInstance::EnumerateInstanceExtensions(const char* layerName)
{
    std::vector<VkExtensionProperties> extensions;
    uint32_t extensionCount = 0;
    VkResult result = VK_SUCCESS;
    do {
        VK_CHECK(vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, nullptr));
        if (extensionCount > 0)
        {
            extensions.resize(extensionCount);
            result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, extensions.data());
        }

    } while (result == VK_INCOMPLETE);

    if (result < 0)
    {
        LogVulkan(Warn, "vkEnumerateInstanceExtensionProperties failed with %s",
            string_VkResult(result));
    }

    return extensions;
}
