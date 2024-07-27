#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

class VulkanDevice : public VulkanObject
{
public:
    VulkanDevice(
        rad::Ref<VulkanInstance> instance,
        rad::Ref<VulkanPhysicalDevice> physicalDevice,
        const std::set<std::string>& extensionNames);
    ~VulkanDevice();

    VkDevice GetHandle() const { return m_handle; }
    const VolkDeviceTable* GetFunctionTable() const { return &m_functionTable; }
    VmaAllocator GetAllocator() const { return m_allocator; }

    VulkanPhysicalDevice* GetPhysicalDevice() const { return m_physicalDevice.get(); }
    const char* GetDeviceName() const;
    const VkPhysicalDeviceLimits& GetLimits() const;

    bool IsExtensionSupported(std::string_view extension);

    bool IsQueueFamilySupported(VulkanQueueFamily queueFamily) const;
    uint32_t GetQueueFamilyIndex(VulkanQueueFamily queueFamily) const;
    rad::Ref<VulkanQueue> CreateQueue(VulkanQueueFamily queueFamily);
    bool IsSurfaceSupported(VulkanQueueFamily queueFamily, VulkanSurface* surface);

    rad::Ref<VulkanCommandPool> CreateCommandPool(
        VulkanQueueFamily queueFamily = VulkanQueueFamilyUniversal,
        VkCommandPoolCreateFlags flags = 0);

    // Synchronization and Cache Control
    rad::Ref<VulkanFence> CreateFence(VkFenceCreateFlags flags = 0);
    rad::Ref<VulkanSemaphore> CreateSemaphore(VkSemaphoreCreateFlags flags = 0);
    rad::Ref<VulkanSemaphore> CreateSemaphoreSignaled();
    rad::Ref<VulkanEvent> CreateEvent();
    void WaitIdle();

    // RenderPass
    rad::Ref<VulkanRenderPass> CreateRenderPass(const VkRenderPassCreateInfo& createInfo);
    rad::Ref<VulkanFramebuffer> CreateFramebuffer(
        VulkanRenderPass* renderPass,
        rad::Span<VulkanImageView*> attachments,
        uint32_t width,
        uint32_t height,
        uint32_t layers);

    // Piplines
    rad::Ref<VulkanShaderModule> CreateShaderModule(rad::Span<uint32_t> code);
    rad::Ref<VulkanGraphicsPipeline> CreateGraphicsPipeline(
        const VkGraphicsPipelineCreateInfo& createInfo);
    rad::Ref<VulkanComputePipeline> CreateComputePipeline(
        const VkComputePipelineCreateInfo& createInfo);

    // Resource
    rad::Ref<VulkanBuffer> CreateBuffer(
        const VkBufferCreateInfo& createInfo,
        const VmaAllocationCreateInfo& allocInfo);
    rad::Ref<VulkanBuffer> CreateBuffer(
        const VkBufferCreateInfo& createInfo,
        VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO);
    rad::Ref<VulkanBuffer> CreateBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO);
    rad::Ref<VulkanBuffer> CreateUniformBuffer(VkDeviceSize size, bool isPersistentMapped = false);
    rad::Ref<VulkanBuffer> CreateStagingBuffer(VkDeviceSize size);
    rad::Ref<VulkanBuffer> CreateStorageBuffer(VkDeviceSize size);
    rad::Ref<VulkanBuffer> CreateVertexBuffer(VkDeviceSize size);
    rad::Ref<VulkanBuffer> CreateIndexBuffer(VkDeviceSize size);

    rad::Ref<VulkanImage> CreateImage(
        const VkImageCreateInfo& createInfo,
        const VmaAllocationCreateInfo& allocInfo);
    rad::Ref<VulkanImage> CreateImage(
        const VkImageCreateInfo& createInfo,
        VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO);
    rad::Ref<VulkanImage> CreateImageFromHandle(
        const VkImageCreateInfo& createInfo,
        VkImage imageHandle);
    rad::Ref<VulkanImage> CreateImage2DRenderTarget(
        VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags addUsage = 0);
    rad::Ref<VulkanImage> CreateImage2DDepthStencil(
        VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags addUsage = 0);
    rad::Ref<VulkanImage> CreateImage2DTexture(
        VkFormat format, uint32_t width, uint32_t height, uint32_t mipLevels);

    // Samplers
    rad::Ref<VulkanSampler> CreatSampler(const VkSamplerCreateInfo& createInfo);
    rad::Ref<VulkanSampler> CreatSamplerNearest(
        VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
    rad::Ref<VulkanSampler> CreatSamplerLinear(
        VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        float maxAnisotropy = 0.0f);

    // Resource Binding
    rad::Ref<VulkanDescriptorSetLayout> CreateDescriptorSetLayout(
        rad::Span<VkDescriptorSetLayoutBinding> layoutBindings);
    rad::Ref<VulkanPipelineLayout> CreatePipelineLayout(
        rad::Span<VulkanDescriptorSetLayout*> descSetLayouts,
        rad::Span<VkPushConstantRange> pushConstantRanges = {});
    rad::Ref<VulkanDescriptorPool> CreateDescriptorPool(
        const VkDescriptorPoolCreateInfo& createInfo);
    rad::Ref<VulkanDescriptorPool> CreateDescriptorPool(
        uint32_t maxSets, rad::Span<VkDescriptorPoolSize> poolSizes);

private:
    rad::Ref<VulkanInstance> m_instance;
    rad::Ref<VulkanPhysicalDevice> m_physicalDevice;
    VkDevice m_handle = VK_NULL_HANDLE;
    // Map VulkanQueueFamily to the real queue family index.
    uint32_t m_queueFamilyIndexTable[VulkanQueueFamilyCount];
    std::set<std::string> m_enabledExtensionNames;
    VolkDeviceTable m_functionTable = {};
    VmaAllocator m_allocator = nullptr;

}; // class VulkanDevice

} // namespace rad
