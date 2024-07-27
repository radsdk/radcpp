#define VOLK_IMPLEMENTATION 1
#define VMA_IMPLEMENTATION 1
#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

spdlog::logger* GetVulkanLogger()
{
    static std::shared_ptr<spdlog::logger> logger = CreateLogger("Vulkan");
    return logger.get();
}

void ReportVulkanError(VkResult result, const char* function, const char* file, uint32_t line)
{
    if (result < 0)
    {
        RAD_LOG(GetVulkanLogger(), err, "{} failed with VkResult={} ({}, line {}).",
            function, string_VkResult(result), file, line);
        throw VulkanError(result);
    }
}

bool vkIsVersionMatchOrGreater(uint32_t version, uint32_t major, uint32_t minor, uint32_t patch)
{
    return (VK_VERSION_MAJOR(version) >= major) &&
        (VK_VERSION_MINOR(version) >= minor) &&
        (VK_VERSION_PATCH(version) >= patch);
}

std::vector<VkLayerProperties> vkEnumerateInstanceLayers()
{
    std::vector<VkLayerProperties> layers;
    uint32_t count = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&count, nullptr));
    if (count > 0)
    {
        layers.resize(count);
        VK_CHECK(vkEnumerateInstanceLayerProperties(&count, layers.data()));
    }
    return layers;
}

std::vector<VkExtensionProperties> vkEnumerateInstanceExtensions(const char* layerName)
{
    std::vector<VkExtensionProperties> extensions;
    uint32_t count = 0;
    VK_CHECK(vkEnumerateInstanceExtensionProperties(layerName, &count, nullptr));
    if (count > 0)
    {
        extensions.resize(count);
        VK_CHECK(vkEnumerateInstanceExtensionProperties(layerName, &count, extensions.data()));
    }
    return extensions;
}

bool vkHasLayer(rad::Span<VkLayerProperties> layer, std::string_view name)
{
    for (const VkLayerProperties& layer : layer)
    {
        if (rad::StrEqual(layer.layerName, name))
        {
            return true;
        }
    }
    return false;
}

bool vkHasExtension(rad::Span<VkExtensionProperties> extensions, std::string_view name)
{
    for (const VkExtensionProperties& extension : extensions)
    {
        if (rad::StrEqual(extension.extensionName, name))
        {
            return true;
        }
    }
    return false;
}

uint32_t vkGetMaxMipLevel(uint32_t width, uint32_t height)
{
    uint32_t maxExtent = std::max(width, height);
    return (uint32_t)std::log2f(float(maxExtent)) + 1;
}

uint32_t vkGetMaxMipLevel(uint32_t width, uint32_t height, uint32_t depth)
{
    uint32_t maxExtent = std::max(std::max(width, height), depth);
    return (uint32_t)std::log2f(float(maxExtent)) + 1;
}

VkImageAspectFlags vkGetImageAspectFromFormat(VkFormat format)
{
    switch (format)
    {
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_D32_SFLOAT:
        return VK_IMAGE_ASPECT_DEPTH_BIT;

    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

    default:
        return VK_IMAGE_ASPECT_COLOR_BIT;
    }
}

VulkanObject::VulkanObject()
{
}

VulkanObject::~VulkanObject()
{
}

} // namespace rad
