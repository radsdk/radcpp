#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/Integer.h>
#include <rad/Core/RefCounted.h>
#include <rad/Core/String.h>
#include <rad/Container/Span.h>
#include <rad/IO/Logging.h>
#include <exception>

#define VK_NO_PROTOTYPES 1
#define VK_ENABLE_BETA_EXTENSIONS 1
#include <volk.h>
#include <vk_enum_string_helper.h>
#include <vk_format_utils.h>

// Fetch pointers to Vulkan functions dynamically:
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include <vk_mem_alloc.h>

namespace rad
{

enum VulkanQueueFamily : uint32_t
{
    VulkanQueueFamilyUniversal,     // Universal Engine supports all features (Graphics or Compute).
    VulkanQueueFamilyCompute,       // Async Compute Engine (ACE)
    VulkanQueueFamilyTransfer,      // DMA
    VulkanQueueFamilyVideoDecode,
    VulkanQueueFamilyVideoEncode,
    VulkanQueueFamilyOpticalFlow,
    VulkanQueueFamilyCount,
};

enum VulkanVendorID : uint32_t
{
    VulkanVendorKhronos = VK_VENDOR_ID_KHRONOS,
    VulkanVendorVIV = VK_VENDOR_ID_VIV,
    VulkanVendorVSI = VK_VENDOR_ID_VSI,
    VulkanVendorKazan = VK_VENDOR_ID_KAZAN,
    VulkanVendorCodeplay = VK_VENDOR_ID_CODEPLAY,
    VulkanVendorMesa = VK_VENDOR_ID_MESA,
    VulkanVendorPOCL = VK_VENDOR_ID_POCL,
    VulkanVendorMobileye = VK_VENDOR_ID_MOBILEYE,
    VulkanVendorAMD1 = 0x1002,
    VulkanVendorAMD2 = 0x1022,
    VulkanVendorNVIDIA = 0x10DE,
    VulkanVendorIntel = 0x8086,
};

class VulkanError : public std::exception
{
public:
    VulkanError(VkResult result) : m_result(result) {}
    const char* what() const noexcept override { return string_VkResult(m_result); }
private:
    VkResult m_result;
}; // class VulkanError

spdlog::logger* GetVulkanLogger();

// Check Vulkan return code and throw VulkanError if result < 0.
void ReportVulkanError(VkResult result, const char* function, const char* file, uint32_t line);
#define VK_CHECK(VulkanCall) \
    do { const VkResult result = VulkanCall; ReportVulkanError(result, #VulkanCall, __FILE__, __LINE__); } while(0)

#define VK_STRUCTURE_CHAIN_BEGIN(Head) auto Head##ChainNext = &Head.pNext;
#define VK_STRUCTURE_CHAIN_ADD(Head, Next) do { *Head##ChainNext = &Next; Head##ChainNext = &Next.pNext; } while(0)
#define VK_STRUCTURE_CHAIN_END(Head) do { *Head##ChainNext = nullptr; } while(0)

struct VulkanMacro
{
    VulkanMacro()
    {
    }

    VulkanMacro(std::string_view name)
    {
        this->m_name = name;
    }

    VulkanMacro(std::string_view name, std::string_view definition)
    {
        this->m_name = name;
        this->m_definition = definition;
    }

    template<typename T>
    VulkanMacro(std::string_view name, T definition)
    {
        this->m_name = name;
        this->m_definition = std::to_string(definition);
    }

    std::string m_name;
    std::string m_definition;

}; // class VulkanMacro

bool vkIsVersionMatchOrGreater(uint32_t version, uint32_t major, uint32_t minor, uint32_t patch);
std::vector<VkLayerProperties> vkEnumerateInstanceLayers();
std::vector<VkExtensionProperties> vkEnumerateInstanceExtensions(const char* layerName = nullptr);
bool vkHasLayer(rad::Span<VkLayerProperties> extensions, std::string_view name);
bool vkHasExtension(rad::Span<VkExtensionProperties> extensions, std::string_view name);
uint32_t vkGetMaxMipLevel(uint32_t width, uint32_t height);
uint32_t vkGetMaxMipLevel(uint32_t width, uint32_t height, uint32_t depth);
VkImageAspectFlags vkGetImageAspectFromFormat(VkFormat format);

class VulkanObject : public RefCounted<VulkanObject>
{
public:
    VulkanObject();
    virtual ~VulkanObject();

protected:
    // Disable copy and move.
    VulkanObject(const VulkanObject&) = delete;
    VulkanObject& operator=(const VulkanObject&) = delete;
    VulkanObject(VulkanObject&&) = delete;
    VulkanObject& operator=(VulkanObject&&) = delete;

}; // class VulkanObject

class VulkanInstance;
class VulkanPhysicalDevice;
class VulkanDevice;
class VulkanQueue;
class VulkanCommandPool;
class VulkanCommandBuffer;
class VulkanFence;
class VulkanSemaphore;
class VulkanEvent;
class VulkanRenderPass;
class VulkanFramebuffer;
class VulkanShaderModule;
class VulkanPipelineLayout;
class VulkanPipeline;
class VulkanGraphicsPipeline;
class VulkanComputePipeline;
class VulkanBuffer;
class VulkanBufferView;
class VulkanImage;
class VulkanImageView;
class VulkanSampler;
class VulkanDescriptorPool;
class VulkanDescriptorSetLayout;
class VulkanDescriptorSet;
class VulkanSurface;
class VulkanSwapchain;

} // namespace rad
