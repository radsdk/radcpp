#include "VulkanImage.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanQueue.h"
#include "VulkanCommandBuffer.h"
#include "VulkanBuffer.h"
#include "VulkanFormat.h"

VulkanImage::VulkanImage(rad::Ref<VulkanDevice> device, const VkImageCreateInfo& createInfo,
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
        vmaGetMemoryTypeProperties(m_device->GetAllocator(), m_allocationInfo.memoryType, &m_memoryFlags);
    }

    m_defaultView = CreateDefaultView();
}

VulkanImage::VulkanImage(rad::Ref<VulkanDevice> device, VkImage handle,
    const VkImageCreateInfo& createInfo) :
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

    m_defaultView = CreateDefaultView();
}

VulkanImage::~VulkanImage()
{
    // If there is no allocation, the image is swapchain image and the resource is not managed.
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

    return new VulkanImageView(m_device, this, createInfo);
}

rad::Ref<VulkanImageView> VulkanImage::CreateImageView2D(
    uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer)
{
    VkImageSubresourceRange range = {};
    range.aspectMask = VulkanFormat(m_format).GetAspectFlags();
    range.baseMipLevel = baseMipLevel;
    range.levelCount = levelCount;
    range.baseArrayLayer = baseArrayLayer;
    range.layerCount = 1;
    return CreateImageView(VK_IMAGE_VIEW_TYPE_2D, m_format, range);
}

void VulkanImage::CopyFromBuffer(VulkanBuffer* buffer, rad::Span<VkBufferImageCopy> copyInfos)
{
    rad::Ref<VulkanCommandBuffer> commandBuffer = m_device->AllocateCommandBufferOneTimeUse();
    commandBuffer->Begin();
    commandBuffer->TransitLayout(this,
        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_ACCESS_NONE, VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    commandBuffer->CopyBufferToImage(buffer, this, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, copyInfos);
    commandBuffer->TransitLayout(this,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_MEMORY_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    commandBuffer->End();

    m_device->GetQueue()->SubmitAndWaitForCompletion(commandBuffer.get());
}

void VulkanImage::CopyFromBuffer2D(VulkanBuffer* buffer, VkDeviceSize bufferOffset,
    uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    std::vector<VkBufferImageCopy> copyInfos(levelCount);
    VkExtent3D blockExtent = VulkanFormat(m_format).GetTexelBlockExtent();
    uint32_t blockSize = VulkanFormat(m_format).GetElementSize();
    for (uint32_t mipLevel = baseMipLevel; mipLevel < levelCount; mipLevel++)
    {
        uint32_t mipWidth = std::max<uint32_t>(m_extent.width >> mipLevel, 1);
        uint32_t mipHeight = std::max<uint32_t>(m_extent.height >> mipLevel, 1);

        copyInfos[mipLevel].bufferOffset = bufferOffset;
        copyInfos[mipLevel].bufferRowLength = rad::RoundUpToMultiple(mipWidth, blockExtent.width);
        copyInfos[mipLevel].bufferImageHeight = rad::RoundUpToMultiple(mipHeight, blockExtent.height);
        copyInfos[mipLevel].imageSubresource.aspectMask = VulkanFormat(m_format).GetAspectFlags();
        copyInfos[mipLevel].imageSubresource.mipLevel = mipLevel;
        copyInfos[mipLevel].imageSubresource.baseArrayLayer = baseArrayLayer;
        copyInfos[mipLevel].imageSubresource.layerCount = layerCount;
        copyInfos[mipLevel].imageOffset = {};
        copyInfos[mipLevel].imageExtent.width = mipWidth;
        copyInfos[mipLevel].imageExtent.height = mipHeight;
        copyInfos[mipLevel].imageExtent.depth = 1;

        bufferOffset += (copyInfos[mipLevel].bufferRowLength / blockExtent.width) *
            (copyInfos[mipLevel].bufferImageHeight / blockExtent.height) * blockSize * layerCount;
    }
    CopyFromBuffer(buffer, copyInfos);
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
    wholeRange.aspectMask = VulkanFormat(m_format).GetAspectFlags();
    wholeRange.baseMipLevel = 0;
    wholeRange.levelCount = m_mipLevels;
    wholeRange.baseArrayLayer = 0;
    wholeRange.layerCount = m_arrayLayers;

    return CreateImageView(viewType, m_format, wholeRange);
}

VulkanImageView::VulkanImageView(rad::Ref<VulkanDevice> device, rad::Ref<VulkanImage> image,
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
