#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include "VulkanCommon.h"
#include <set>

class VulkanDevice : public VulkanObject
{
public:
    VulkanDevice(
        rad::Ref<VulkanInstance> instance,
        rad::Ref<VulkanPhysicalDevice> physicalDevice,
        std::set<std::string> extensionNames);
    ~VulkanDevice();

    VkDevice GetHandle() const { return m_handle; }
    const VolkDeviceTable* GetFunctionTable() const { return &m_functionTable; }
    VmaAllocator GetAllocator() const { return m_allocator; }

    VulkanPhysicalDevice* GetPhysicalDevice() const { return m_physicalDevice.get(); }
    const VkPhysicalDeviceLimits& GetLimits() const;

    bool SupportsExtension(std::string_view extension);

    uint32_t GetQueueFamilyIndex(VulkanQueueFamily queueFamily);
    VulkanQueue* GetQueue(VulkanQueueFamily queueFamily = VulkanQueueFamilyUniversal);
    bool SupportsSurface(VulkanQueueFamily queueFamily, VkSurfaceKHR surface) const;

    rad::Ref<VulkanCommandPool> CreateCommandPool(
        VulkanQueueFamily queueFamily = VulkanQueueFamilyUniversal,
        VkCommandPoolCreateFlags flags = 0);
    // Allocate a command buffer from the internal CommandPool created with VK_COMMAND_POOL_CREATE_TRANSIENT_BIT.
    rad::Ref<VulkanCommandBuffer> AllocateCommandBufferOneTimeUse(
        VulkanQueueFamily queueFamily = VulkanQueueFamilyUniversal);

    // Synchronization and Cache Control
    rad::Ref<VulkanFence> CreateFence(VkFenceCreateFlags flags = 0);
    rad::Ref<VulkanSemaphore> CreateSemaphore(VkSemaphoreCreateFlags flags = 0);
    rad::Ref<VulkanSemaphore> CreateSemaphoreSignaled();
    rad::Ref<VulkanSemaphore> CreateSemaphoreFromHandle(VkSemaphore semaphoreHandle);
    rad::Ref<VulkanEvent> CreateEvent();
    void WaitIdle();

    // RenderPass
    rad::Ref<VulkanRenderPass> CreateRenderPass(const VkRenderPassCreateInfo& createInfo);
    rad::Ref<VulkanFramebuffer> CreateFramebuffer(
        VulkanRenderPass* renderPass,
        rad::Span<VulkanImageView*> attachments,
        uint32_t width,
        uint32_t height,
        uint32_t layers = 1
    );

    // Piplines
    rad::Ref<VulkanShaderModule> CreateShaderModule(rad::Span<uint32_t> code);
    rad::Ref<VulkanGraphicsPipeline> CreateGraphicsPipeline(const VulkanGraphicsPipelineCreateInfo& createInfo);
    rad::Ref<VulkanComputePipeline> CreateComputePipeline(const VulkanComputePipelineCreateInfo& createInfo);

    // Resource Creation
    rad::Ref<VulkanBuffer> CreateBuffer(const VkBufferCreateInfo& createInfo, const VmaAllocationCreateInfo& allocInfo);
    rad::Ref<VulkanBuffer> CreateBuffer(
        const VkBufferCreateInfo& createInfo, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO);
    rad::Ref<VulkanBuffer> CreateBuffer(
        VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO);
    rad::Ref<VulkanBuffer> CreateUniformBuffer(VkDeviceSize size, bool isPersistentMapped = false);
    rad::Ref<VulkanBuffer> CreateStagingBuffer(VkDeviceSize size);
    rad::Ref<VulkanBuffer> CreateStorageBuffer(VkDeviceSize size, VkBufferUsageFlags addUsage = 0);
    rad::Ref<VulkanBuffer> CreateVertexBuffer(VkDeviceSize size, VkBufferUsageFlags addUsage = 0);
    rad::Ref<VulkanBuffer> CreateIndexBuffer(VkDeviceSize size, VkBufferUsageFlags addUsage = 0);
    rad::Ref<VulkanImage> CreateImage(const VkImageCreateInfo& createInfo, const VmaAllocationCreateInfo& allocInfo);
    rad::Ref<VulkanImage> CreateImage(const VkImageCreateInfo& createInfo, VmaMemoryUsage usage = VMA_MEMORY_USAGE_AUTO);
    rad::Ref<VulkanImage> CreateImageFromHandle(VkImage imageHandle, const VkImageCreateInfo& createInfo);
    rad::Ref<VulkanImage> CreateImage2DRenderTarget(
        VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags addUsage = 0);
    rad::Ref<VulkanImage> CreateImage2DDepthStencil(
        VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags addUsage = 0);
    rad::Ref<VulkanImage> CreateImage2DTexture(
        VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags addUsage = 0);

    // Samplers
    rad::Ref<VulkanSampler> CreatSampler(const VkSamplerCreateInfo& createInfo);
    rad::Ref<VulkanSampler> CreatSamplerNearest(
        VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
    rad::Ref<VulkanSampler> CreatSamplerLinear(
        VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        float maxAnisotropy = 0.0f);

    // Resource Descriptors
    rad::Ref<VulkanDescriptorSetLayout> CreateDescriptorSetLayout(
        rad::Span<VkDescriptorSetLayoutBinding> layoutBindings);
    rad::Ref<VulkanPipelineLayout> CreatePipelineLayout(
        rad::Span<VulkanDescriptorSetLayout*> descSetLayouts,
        rad::Span<VkPushConstantRange> pushConstantRanges = {});
    rad::Ref<VulkanDescriptorPool> CreateDescriptorPool(const VkDescriptorPoolCreateInfo& createInfo);
    rad::Ref<VulkanDescriptorPool> CreateDescriptorPool(
        uint32_t maxSets, rad::Span<VkDescriptorPoolSize> poolSizes);

    rad::Ref<VulkanSwapchain> CreateSwapchain(const VkSwapchainCreateInfoKHR& createInfo);

private:
    rad::Ref<VulkanInstance> m_instance;
    rad::Ref<VulkanPhysicalDevice> m_physicalDevice;
    VkDevice m_handle = VK_NULL_HANDLE;
    VolkDeviceTable m_functionTable;
    // manage device memory allocations
    VmaAllocator m_allocator = nullptr;

    uint32_t m_queueFamilyIndices[VulkanQueueFamilyCount];
    rad::Ref<VulkanQueue> m_queues[VulkanQueueFamilyCount];
    rad::Ref<VulkanCommandPool> m_commandPoolsTransientAlloc[VulkanQueueFamilyCount];

    std::set<std::string> m_enabledExtensionNames;

}; // class VulkanDevice

#endif // VULKAN_DEVICE_H
