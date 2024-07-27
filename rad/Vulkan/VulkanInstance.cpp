#include <rad/Vulkan/VulkanInstance.h>
#include <rad/Vulkan/VulkanPhysicalDevice.h>
#include <SDL3/SDL_vulkan.h>

namespace rad
{

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

    if (m_handle != VK_NULL_HANDLE)
    {
        vkDestroyInstance(m_handle, nullptr);
        m_handle = VK_NULL_HANDLE;
    }

    SDL_Vulkan_UnloadLibrary();
}

bool VulkanInstance::IsVersionMatchOrGreater(uint32_t major, uint32_t minor, uint32_t patch)
{
    return vkIsVersionMatchOrGreater(m_version, major, minor, patch);
}

VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugUtilsMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    switch (severity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        RAD_LOG(GetVulkanLogger(), debug, "[{}] {}",
            pCallbackData->pMessageIdName, pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        RAD_LOG(GetVulkanLogger(), info, "[{}] {}",
            pCallbackData->pMessageIdName, pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        RAD_LOG(GetVulkanLogger(), warn, "[{}] {}",
            pCallbackData->pMessageIdName, pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        RAD_LOG(GetVulkanLogger(), err, "[{}] {}",
            pCallbackData->pMessageIdName, pCallbackData->pMessage);
#if defined(RAD_COMPILER_MSVC) && defined(_DEBUG)
        __debugbreak();
#endif
        break;
    }

    return VK_FALSE;
}

bool VulkanInstance::Init(std::string_view appName, uint32_t appVersion)
{
    int result = SDL_Vulkan_LoadLibrary(nullptr);
    if (result != 0)
    {
        RAD_LOG(GetVulkanLogger(), err, "SDL_Vulkan_LoadLibrary failed: {}", SDL_GetError());
        return false;
    }

    VK_CHECK(volkInitialize());

    if (vkEnumerateInstanceVersion)
    {
        vkEnumerateInstanceVersion(&m_version);
        RAD_LOG(GetVulkanLogger(), info, "Vulkan instance level API version: {}.{}.{}.{}",
            VK_API_VERSION_VARIANT(m_version),
            VK_VERSION_MAJOR(m_version),
            VK_VERSION_MINOR(m_version),
            VK_VERSION_PATCH(m_version)
        );
    }

    std::vector<const char*> enabledLayers;

    std::vector<VkLayerProperties> layers = vkEnumerateInstanceLayers();

#ifdef _DEBUG
    bool enableValidation = true;
#else
    bool enableValidation = false;
#endif

    if (enableValidation)
    {
        const char* pValidationLayerName = "VK_LAYER_KHRONOS_validation";
        bool layerFound = false;
        for (const VkLayerProperties& layer : layers)
        {
            if (rad::StrEqual(layer.layerName, pValidationLayerName))
            {
                enabledLayers.push_back(pValidationLayerName);
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            RAD_LOG(GetVulkanLogger(), warn, "Cannot find validation layer {}",
                pValidationLayerName);
            enableValidation = false;
        }
    }

    std::vector<VkExtensionProperties> extensions = vkEnumerateInstanceExtensions(nullptr);
    Uint32 count = 0;
    const char* const* sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&count);
    for (Uint32 i = 0; i < count; ++i)
    {
        if (vkHasExtension(extensions, sdlExtensions[i]))
        {
            m_enabledExtensions.insert(sdlExtensions[i]);
        }
        else
        {
            RAD_LOG(GetVulkanLogger(), warn, "SDL required extension is not supported: {}",
                sdlExtensions[i]);
        }
    }

    if (vkHasExtension(extensions, "VK_KHR_get_physical_device_properties2"))
    {
        m_enabledExtensions.insert("VK_KHR_get_physical_device_properties2");
    }

    if (vkHasExtension(extensions, "VK_KHR_get_surface_capabilities2"))
    {
        m_enabledExtensions.insert("VK_KHR_get_surface_capabilities2");
    }

    if (enableValidation)
    {
        if (vkHasExtension(extensions, VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
        {
            m_enabledExtensions.insert(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        else
        {
            RAD_LOG(GetVulkanLogger(), warn, "Cannot enable validation layer: "
                "missing layer extension {}",
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            enableValidation = false;
        }
    }

    std::vector<const char*> enabledExtensions;
    for (const std::string& extension : m_enabledExtensions)
    {
        enabledExtensions.push_back(extension.data());
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = appName.data();
    appInfo.applicationVersion = appVersion;
    appInfo.pEngineName = appName.data();
    appInfo.engineVersion = appVersion;
    appInfo.apiVersion = m_version;

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
    instanceCreateInfo.ppEnabledLayerNames = enabledLayers.data();
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
    if (enableValidation)
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
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
        debugMessengerCreateInfo.pfnUserCallback = vkDebugUtilsMessengerCallback;
        debugMessengerCreateInfo.pUserData = nullptr;
        instanceCreateInfo.pNext = &debugMessengerCreateInfo;
    }

    VK_CHECK(vkCreateInstance(&instanceCreateInfo, nullptr, &m_handle));
    if (m_handle)
    {
        volkLoadInstance(m_handle);
    }

    if (enableValidation)
    {
        VK_CHECK(vkCreateDebugUtilsMessengerEXT(m_handle,
            &debugMessengerCreateInfo, nullptr, &m_debugMessenger));
    }

    return true;
}

bool VulkanInstance::IsExtensionSupported(std::string_view name) const
{
    return m_enabledExtensions.contains(name);
}

std::vector<Ref<VulkanPhysicalDevice>> VulkanInstance::EnumeratePhysicalDevices()
{
    std::vector<Ref<VulkanPhysicalDevice>> devices;
    uint32_t deviceCount = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(m_handle, &deviceCount, nullptr));
    if (deviceCount > 0)
    {
        std::vector<VkPhysicalDevice> deviceHandles(deviceCount);
        VK_CHECK(vkEnumeratePhysicalDevices(m_handle, &deviceCount, deviceHandles.data()));
        devices.resize(deviceCount);
        for (uint32_t i = 0; i < deviceCount; ++i)
        {
            devices[i] = RAD_NEW VulkanPhysicalDevice(this, deviceHandles[i]);
        }
    }
    return devices;
}

} // namespace rad
