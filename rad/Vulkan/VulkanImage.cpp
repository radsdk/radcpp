#include <rad/Vulkan/VulkanImage.h>
#include <rad/Vulkan/VulkanDevice.h>

namespace rad
{

VulkanImage::VulkanImage(
    rad::Ref<VulkanDevice> device,
    const VkImageCreateInfo& createInfo,
    const VmaAllocationCreateInfo& allocInfo) :
    m_device(std::move(device))
{
    m_createFlags = createInfo.flags;
    m_type = createInfo.imageType;
    m_format = createInfo.format;
    m_extent = createInfo.extent;
    m_mipLevels = createInfo.mipLevels;
    m_arrayLayers = createInfo.arrayLayers;
    m_samples = createInfo.samples;
    m_tiling = createInfo.tiling;
    m_usage = createInfo.usage;
    m_sharingMode = createInfo.sharingMode;

    VK_CHECK(vmaCreateImage(m_device->GetAllocator(), &createInfo, &allocInfo,
        &m_handle, &m_allocation, &m_allocationInfo));

    if (m_handle != VK_NULL_HANDLE)
    {
        vmaGetMemoryTypeProperties(
            m_device->GetAllocator(), m_allocationInfo.memoryType, &m_memoryFlags);
    }
}

VulkanImage::VulkanImage(
    rad::Ref<VulkanDevice> device,
    const VkImageCreateInfo& createInfo,
    VkImage handle) :
    m_device(std::move(device)),
    m_handle(handle)
{
    m_type = VK_IMAGE_TYPE_2D;
    m_format = createInfo.format;
    m_extent.width = createInfo.extent.width;
    m_extent.height = createInfo.extent.height;
    m_extent.depth = 1;
    m_mipLevels = 1;
    m_arrayLayers = createInfo.arrayLayers;
    m_samples = VK_SAMPLE_COUNT_1_BIT;
    m_tiling = VK_IMAGE_TILING_OPTIMAL;
    m_usage = createInfo.usage;
    m_sharingMode = createInfo.sharingMode;

    m_allocation = nullptr;
    m_memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}

VulkanImage::~VulkanImage()
{
    if (m_handle && m_allocation)
    {
        vmaDestroyImage(m_device->GetAllocator(), m_handle, m_allocation);
        m_handle = VK_NULL_HANDLE;
        m_allocation = VK_NULL_HANDLE;
    }
}

rad::Ref<VulkanImageView> VulkanImage::CreateImageView(
    VkImageViewType                     type,
    VkFormat                            format,
    const VkImageSubresourceRange& subresourceRange,
    const VkComponentMapping* componentMapping)
{
    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0; // reserved for future use
    createInfo.image = m_handle;
    createInfo.viewType = type;
    createInfo.format = format;
    if (componentMapping)
    {
        createInfo.components = *componentMapping;
    }
    else
    {
        createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    }
    createInfo.subresourceRange = subresourceRange;

    return RAD_NEW VulkanImageView(m_device, this, createInfo);
}

rad::Ref<VulkanImageView> VulkanImage::CreateImageView2D(
    uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer)
{
    VkImageSubresourceRange range = {};
    range.aspectMask = vkGetImageAspectFromFormat(m_format);
    range.baseMipLevel = baseMipLevel;
    range.levelCount = levelCount;
    range.baseArrayLayer = baseArrayLayer;
    range.layerCount = 1;
    return CreateImageView(VK_IMAGE_VIEW_TYPE_2D, m_format, range);
}

rad::Ref<VulkanImageView> VulkanImage::CreateDefaultView()
{
    VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_1D;
    if (m_type == VkImageType::VK_IMAGE_TYPE_1D)
    {
        viewType = VK_IMAGE_VIEW_TYPE_1D;
        if (m_arrayLayers > 1)
        {
            viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        }
    }
    else if (m_type == VkImageType::VK_IMAGE_TYPE_2D)
    {
        viewType = VK_IMAGE_VIEW_TYPE_2D;
        if (m_arrayLayers > 1)
        {
            viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            if (m_createFlags | VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT)
            {
                assert(m_arrayLayers % 6 == 0);
                if (m_arrayLayers == 6)
                {
                    viewType = VK_IMAGE_VIEW_TYPE_CUBE;
                }
                else
                {
                    viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
                }
            }
        }
    }
    else if (m_type == VkImageType::VK_IMAGE_TYPE_3D)
    {
        viewType = VK_IMAGE_VIEW_TYPE_3D;
    }

    VkImageSubresourceRange wholeRange = {};
    wholeRange.aspectMask = vkGetImageAspectFromFormat(m_format);
    wholeRange.baseMipLevel = 0;
    wholeRange.levelCount = m_mipLevels;
    wholeRange.baseArrayLayer = 0;
    wholeRange.layerCount = m_arrayLayers;

    return CreateImageView(viewType, m_format, wholeRange);
}

VulkanImageView::VulkanImageView(
    rad::Ref<VulkanDevice> device,
    rad::Ref<VulkanImage> image,
    const VkImageViewCreateInfo& createInfo) :
    m_device(std::move(device)),
    m_image(std::move(image))
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateImageView(m_device->GetHandle(), &createInfo, nullptr, &m_handle));
}

VulkanImageView::~VulkanImageView()
{
    m_device->GetFunctionTable()->
        vkDestroyImageView(m_device->GetHandle(), m_handle, nullptr);
    m_handle = VK_NULL_HANDLE;
}

} // namespace rad
