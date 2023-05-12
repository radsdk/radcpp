#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"
#include "VulkanDevice.h"
#include "VulkanEvent.h"
#include "VulkanFence.h"
#include "VulkanFormat.h"
#include "VulkanFramebuffer.h"
#include "VulkanImage.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanPipeline.h"
#include "VulkanQueue.h"
#include "VulkanRenderPass.h"
#include "VulkanSampler.h"
#include "VulkanSemaphore.h"
#include "VulkanShader.h"
#include "VulkanSwapchain.h"

VulkanDevice::VulkanDevice(
    rad::Ref<VulkanInstance>            instance,
    rad::Ref<VulkanPhysicalDevice>      physicalDevice,
    std::set<std::string>               extensionNames) :
    m_instance(std::move(instance)),
    m_physicalDevice(std::move(physicalDevice))
{
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext;
    createInfo.flags;

    for (size_t i = 0; i < VulkanQueueFamilyCount; ++i)
    {
        m_queueFamilyIndices[i] = VK_QUEUE_FAMILY_IGNORED;
    }

    const std::vector<VkQueueFamilyProperties>& queueFamilyProps =
        m_physicalDevice->GetQueueFamilyProperties();
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    float queuePriorities[1] = { 1.0f };
    for (uint32_t i = 0; i < queueFamilyProps.size(); ++i)
    {
        LogVulkan(Debug, "QueueFamily#%u: %s", i,
            string_VkQueueFlags(queueFamilyProps[i].queueFlags).c_str());

        rad::Flags32<VkQueueFlagBits> queueFlags(queueFamilyProps[i].queueFlags);

        if ((m_queueFamilyIndices[VulkanQueueFamilyUniversal] == VK_QUEUE_FAMILY_IGNORED)
            && queueFlags.HasBits(VK_QUEUE_GRAPHICS_BIT))
        {
            m_queueFamilyIndices[VulkanQueueFamilyUniversal] = i;
            VkDeviceQueueCreateInfo queueInfo = {};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.pNext;
            queueInfo.flags;
            queueInfo.queueFamilyIndex = i;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = queuePriorities;
            queueInfos.push_back(queueInfo);
        }
        if ((m_queueFamilyIndices[VulkanQueueFamilyCompute] == VK_QUEUE_FAMILY_IGNORED)
            && queueFlags.HasBits(VK_QUEUE_COMPUTE_BIT)
            && queueFlags.HasNoBits(VK_QUEUE_GRAPHICS_BIT))
        {
            m_queueFamilyIndices[VulkanQueueFamilyCompute] = i;
            VkDeviceQueueCreateInfo queueInfo = {};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.pNext;
            queueInfo.flags;
            queueInfo.queueFamilyIndex = i;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = queuePriorities;
            queueInfos.push_back(queueInfo);
        }
        if ((m_queueFamilyIndices[VulkanQueueFamilyTransfer] == VK_QUEUE_FAMILY_IGNORED)
            && queueFlags.HasBits(VK_QUEUE_TRANSFER_BIT)
            && queueFlags.HasNoBits(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT))
        {
            m_queueFamilyIndices[VulkanQueueFamilyTransfer] = i;
            VkDeviceQueueCreateInfo queueInfo = {};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.pNext;
            queueInfo.flags;
            queueInfo.queueFamilyIndex = i;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = queuePriorities;
            queueInfos.push_back(queueInfo);
        }
    }

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
    createInfo.pQueueCreateInfos = queueInfos.data();
    createInfo.enabledLayerCount = 0; // is deprecated and ignored.
    createInfo.ppEnabledLayerNames = nullptr; // is deprecated and ignored.
    m_enabledExtensionNames = extensionNames;
    std::vector<const char*> enabledExtensionNames;
    enabledExtensionNames.reserve(m_enabledExtensionNames.size());
    for (const std::string& extensionName : m_enabledExtensionNames)
    {
        enabledExtensionNames.push_back(extensionName.data());
    }
    createInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensionNames.size());
    createInfo.ppEnabledExtensionNames = enabledExtensionNames.data();
    createInfo.pEnabledFeatures = nullptr;

    const VkPhysicalDeviceFeatures2& features2 = m_physicalDevice->GetFeatures2();
    createInfo.pNext = &features2;

    VK_CHECK(vkCreateDevice(m_physicalDevice->GetHandle(), &createInfo, nullptr, &m_handle));
    if (m_handle)
    {
        volkLoadDeviceTable(&m_functionTable, m_handle);

        // Vma Initialization
        // https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/quick_start.html#quick_start_initialization
        VmaAllocatorCreateInfo allocatorCreateInfo = {};
        allocatorCreateInfo.vulkanApiVersion = m_instance->GetApiVersion();
        allocatorCreateInfo.instance = m_instance->GetHandle();
        allocatorCreateInfo.physicalDevice = m_physicalDevice->GetHandle();
        allocatorCreateInfo.device = m_handle;
        VmaVulkanFunctions vmaFunctions = {};
        vmaFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
        vmaFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
        allocatorCreateInfo.pVulkanFunctions = &vmaFunctions;
        if (m_physicalDevice->GetVulkan12Features().bufferDeviceAddress)
        {
            allocatorCreateInfo.flags |= VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
        }
        VK_CHECK(vmaCreateAllocator(&allocatorCreateInfo, &m_allocator));
    }

    for (uint32_t i = 0; i < VulkanQueueFamilyCount; ++i)
    {
        VulkanQueueFamily queueFamily = static_cast<VulkanQueueFamily>(i);
        m_queues[i] = new VulkanQueue(this, queueFamily);
        m_commandPoolsTransientAlloc[queueFamily] =
            CreateCommandPool(queueFamily, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
    }
}

VulkanDevice::~VulkanDevice()
{
    vmaDestroyAllocator(m_allocator);
    m_allocator = VK_NULL_HANDLE;
    GetFunctionTable()->
        vkDestroyDevice(m_handle, nullptr);
    m_handle = VK_NULL_HANDLE;
}

const VkPhysicalDeviceLimits& VulkanDevice::GetLimits() const
{
    return m_physicalDevice->GetProperties().limits;
}

bool VulkanDevice::SupportsExtension(std::string_view extension)
{
    for (const auto& extensionName : m_enabledExtensionNames)
    {
        if (extensionName == extension)
        {
            return true;
        }
    }
    return false;
}

uint32_t VulkanDevice::GetQueueFamilyIndex(VulkanQueueFamily queueFamily)
{
    return m_queueFamilyIndices[queueFamily];
}

VulkanQueue* VulkanDevice::GetQueue(VulkanQueueFamily queueFamily)
{
    return m_queues[queueFamily].get();
}

bool VulkanDevice::SupportsSurface(VulkanQueueFamily queueFamily, VkSurfaceKHR surface) const
{
    return m_physicalDevice->SupportsSurface(m_queueFamilyIndices[queueFamily], surface);
}

rad::Ref<VulkanCommandPool>
VulkanDevice::CreateCommandPool(VulkanQueueFamily queueFamily, VkCommandPoolCreateFlags flags)
{
    VkCommandPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = flags;
    createInfo.queueFamilyIndex = GetQueueFamilyIndex(queueFamily);
    return new VulkanCommandPool(this, createInfo);
}

rad::Ref<VulkanCommandBuffer> VulkanDevice::AllocateCommandBufferOneTimeUse(VulkanQueueFamily queueFamily)
{
    return m_commandPoolsTransientAlloc[queueFamily]->Allocate();
}

rad::Ref<VulkanFence> VulkanDevice::CreateFence(VkFenceCreateFlags flags)
{
    VkFenceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = flags;
    return new VulkanFence(this, createInfo);
}

rad::Ref<VulkanSemaphore> VulkanDevice::CreateSemaphore(VkSemaphoreCreateFlags flags)
{
    VkSemaphoreCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0; // reserved for future use
    return new VulkanSemaphore(this, createInfo);
}

rad::Ref<VulkanSemaphore> VulkanDevice::CreateSemaphoreSignaled()
{
    return CreateSemaphore(VK_FENCE_CREATE_SIGNALED_BIT);
}

rad::Ref<VulkanSemaphore> VulkanDevice::CreateSemaphoreFromHandle(VkSemaphore semaphoreHandle)
{
    return new VulkanSemaphore(this, semaphoreHandle);
}

rad::Ref<VulkanEvent> VulkanDevice::CreateEvent()
{
    VkEventCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0; // reserved for future use
    return new VulkanEvent(this, createInfo);
}

void VulkanDevice::WaitIdle()
{
    VK_CHECK(GetFunctionTable()->
        vkDeviceWaitIdle(m_handle));
}

rad::Ref<VulkanRenderPass>
VulkanDevice::CreateRenderPass(const VkRenderPassCreateInfo& createInfo)
{
    return new VulkanRenderPass(this, createInfo);
}

rad::Ref<VulkanFramebuffer> VulkanDevice::CreateFramebuffer(
    VulkanRenderPass* renderPass,
    rad::Span<VulkanImageView*> attachments,
    uint32_t width, uint32_t height, uint32_t layers)
{
    std::vector<VkImageView> attachmenstHandles(attachments.size());
    for (size_t i = 0; i < attachments.size(); ++i)
    {
        attachmenstHandles[i] = attachments[i]->GetHandle();
    }

    VkFramebufferCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.renderPass = renderPass->GetHandle();
    createInfo.attachmentCount = static_cast<uint32_t>(attachmenstHandles.size());
    createInfo.pAttachments = attachmenstHandles.data();
    createInfo.width = width;
    createInfo.height = height;
    createInfo.layers = layers;

    return new VulkanFramebuffer(this, createInfo);
}

rad::Ref<VulkanShaderModule> VulkanDevice::CreateShaderModule(rad::Span<uint32_t> code)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.codeSize = code.size() * sizeof(uint32_t);
    createInfo.pCode = code.data();

    return new VulkanShaderModule(this, createInfo);
}

rad::Ref<VulkanGraphicsPipeline>
VulkanDevice::CreateGraphicsPipeline(const VulkanGraphicsPipelineCreateInfo& createInfo)
{
    return new VulkanGraphicsPipeline(this, createInfo);
}

rad::Ref<VulkanComputePipeline>
VulkanDevice::CreateComputePipeline(const VulkanComputePipelineCreateInfo& createInfo)
{
    return new VulkanComputePipeline(this, createInfo);
}

rad::Ref<VulkanBuffer>
VulkanDevice::CreateBuffer(const VkBufferCreateInfo& createInfo, const VmaAllocationCreateInfo& allocInfo)
{
    return new VulkanBuffer(this, createInfo, allocInfo);
}

rad::Ref<VulkanBuffer> VulkanDevice::CreateBuffer(const VkBufferCreateInfo& createInfo, VmaMemoryUsage memoryUsage)
{
    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = memoryUsage;
    return CreateBuffer(createInfo, allocInfo);
}

rad::Ref<VulkanBuffer> VulkanDevice::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
{
    VkBufferCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size = size;
    createInfo.usage = usage;
    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = memoryUsage;
    return CreateBuffer(createInfo, allocInfo);
}

rad::Ref<VulkanBuffer> VulkanDevice::CreateUniformBuffer(VkDeviceSize size, bool isPersistentMapped)
{
    VkBufferCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size = size;
    createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    if (isPersistentMapped)
    {
        allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
    }
    return CreateBuffer(createInfo, allocInfo);
}

rad::Ref<VulkanBuffer> VulkanDevice::CreateStagingBuffer(VkDeviceSize size)
{
    VkBufferUsageFlags usage =
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    return CreateBuffer(size, usage, VMA_MEMORY_USAGE_CPU_ONLY);
}

rad::Ref<VulkanBuffer> VulkanDevice::CreateStorageBuffer(VkDeviceSize size, VkBufferUsageFlags addUsage)
{
    VkBufferCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size = size;
    createInfo.usage =
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
        addUsage;
    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    return new VulkanBuffer(this, createInfo, allocInfo);
}

rad::Ref<VulkanBuffer> VulkanDevice::CreateVertexBuffer(VkDeviceSize size, VkBufferUsageFlags addUsage)
{
    VkBufferUsageFlags usage =
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
        addUsage;
    return CreateBuffer(size, usage, VMA_MEMORY_USAGE_GPU_ONLY);
}

rad::Ref<VulkanBuffer> VulkanDevice::CreateIndexBuffer(VkDeviceSize size, VkBufferUsageFlags addUsage)
{
    VkBufferUsageFlags usage =
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
        addUsage;
    return CreateBuffer(size, usage, VMA_MEMORY_USAGE_GPU_ONLY);
}

rad::Ref<VulkanImage> VulkanDevice::CreateImage(
    const VkImageCreateInfo& createInfo,
    const VmaAllocationCreateInfo& allocInfo)
{
    return new VulkanImage(this, createInfo, allocInfo);
}

rad::Ref<VulkanImage> VulkanDevice::CreateImage(const VkImageCreateInfo& createInfo, VmaMemoryUsage usage)
{
    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = usage;
    return CreateImage(createInfo, allocInfo);
}

rad::Ref<VulkanImage>
VulkanDevice::CreateImageFromHandle(VkImage imageHandle, const VkImageCreateInfo& createInfo)
{
    return new VulkanImage(this, imageHandle, createInfo);
}

rad::Ref<VulkanImage> VulkanDevice::CreateImage2DRenderTarget(
    VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags addUsage)
{
    VkImageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.format = format;
    createInfo.extent.width = width;
    createInfo.extent.height = height;
    createInfo.extent.depth = 1;
    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | addUsage;
    return CreateImage(createInfo, VMA_MEMORY_USAGE_GPU_ONLY);
}

rad::Ref<VulkanImage> VulkanDevice::CreateImage2DDepthStencil(
    VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags addUsage)
{
    VkImageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.format = format;
    createInfo.extent.width = width;
    createInfo.extent.height = height;
    createInfo.extent.depth = 1;
    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | addUsage;
    return CreateImage(createInfo, VMA_MEMORY_USAGE_GPU_ONLY);
}

rad::Ref<VulkanImage> VulkanDevice::CreateImage2DTexture(
    VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlags addUsage)
{
    VkImageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.format = format;
    createInfo.extent.width = width;
    createInfo.extent.height = height;
    createInfo.extent.depth = 1;
    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage =
        VK_IMAGE_USAGE_TRANSFER_DST_BIT |
        VK_IMAGE_USAGE_SAMPLED_BIT |
        addUsage;
    return CreateImage(createInfo, VMA_MEMORY_USAGE_GPU_ONLY);
}

rad::Ref<VulkanSampler> VulkanDevice::CreatSampler(const VkSamplerCreateInfo& createInfo)
{
    return new VulkanSampler(this, createInfo);
}

rad::Ref<VulkanSampler> VulkanDevice::CreatSamplerNearest(VkSamplerAddressMode addressMode)
{
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.pNext = nullptr;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    samplerInfo.addressModeU = addressMode;
    samplerInfo.addressModeV = addressMode;
    samplerInfo.addressModeW = addressMode;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1;
    samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerInfo.minLod = 0;
    samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    return CreatSampler(samplerInfo);
}

rad::Ref<VulkanSampler> VulkanDevice::CreatSamplerLinear(VkSamplerAddressMode addressMode, float maxAnisotropy)
{
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.pNext = nullptr;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = addressMode;
    samplerInfo.addressModeV = addressMode;
    samplerInfo.addressModeW = addressMode;
    samplerInfo.mipLodBias = 0.0f;
    if (maxAnisotropy > 0.0f)
    {
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = maxAnisotropy;
    }
    else
    {
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
    }
    samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerInfo.minLod = 0;
    samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    return CreatSampler(samplerInfo);
}

rad::Ref<VulkanDescriptorSetLayout>
VulkanDevice::CreateDescriptorSetLayout(rad::Span<VkDescriptorSetLayoutBinding> layoutBindings)
{
    VkDescriptorSetLayoutCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
    createInfo.pBindings = layoutBindings.data();

    return new VulkanDescriptorSetLayout(this, createInfo);
}

rad::Ref<VulkanPipelineLayout> VulkanDevice::CreatePipelineLayout(
    rad::Span<VulkanDescriptorSetLayout*> descSetLayouts, rad::Span<VkPushConstantRange> pushConstantRanges)
{
    std::vector<VkDescriptorSetLayout> descSetLayoutsHandles(descSetLayouts.size());
    for (size_t i = 0; i < descSetLayouts.size(); ++i)
    {
        descSetLayoutsHandles[i] = descSetLayouts[i]->GetHandle();
    }

    VkPipelineLayoutCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.setLayoutCount = static_cast<uint32_t>(descSetLayoutsHandles.size());
    createInfo.pSetLayouts = descSetLayoutsHandles.data();
    createInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
    createInfo.pPushConstantRanges = pushConstantRanges.data();

    return new VulkanPipelineLayout(this, createInfo);
}

rad::Ref<VulkanDescriptorPool> VulkanDevice::CreateDescriptorPool(const VkDescriptorPoolCreateInfo& createInfo)
{
    return new VulkanDescriptorPool(this, createInfo);
}

rad::Ref<VulkanDescriptorPool>
VulkanDevice::CreateDescriptorPool(uint32_t maxSets, rad::Span<VkDescriptorPoolSize> poolSizes)
{
    VkDescriptorPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    createInfo.maxSets = maxSets;
    createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    createInfo.pPoolSizes = poolSizes.data();

    return CreateDescriptorPool(createInfo);
}

rad::Ref<VulkanSwapchain> VulkanDevice::CreateSwapchain(const VkSwapchainCreateInfoKHR& createInfo)
{
    return new VulkanSwapchain(this, createInfo);
}
