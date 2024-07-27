#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

class VulkanBuffer : public VulkanObject
{
public:
    VulkanBuffer(
        rad::Ref<VulkanDevice> device,
        const VkBufferCreateInfo& createInfo,
        const VmaAllocationCreateInfo& allocInfo);
    ~VulkanBuffer();

    VkBuffer GetHandle() const { return m_handle; }

    VkDeviceSize GetSize() const { return m_size; }
    VkBufferUsageFlags GetUsage() const { return m_usage; }
    VkMemoryPropertyFlags GetMemoryFlags() const { return m_memoryFlags; }
    bool IsHostVisible() const;
    bool IsHostCoherent() const;
    VmaAllocation GetAllocation() { return m_allocation; }
    const VmaAllocationInfo& GetAllocationInfo() const { return m_allocationInfo; }
    VkDeviceAddress GetDeviceAddress() const;

    void* GetMappedAddr();
    void* MapMemory(VkDeviceSize offset, VkDeviceSize size);
    // Map whole range.
    void* MapMemory();
    void UnmapMemory();
    void FlushAllocation(VkDeviceSize offset, VkDeviceSize size);
    void FlushAllocation();
    void InvalidateAllocation(VkDeviceSize offset, VkDeviceSize size);
    void InvalidateAllocation();

    rad::Ref<VulkanBufferView> CreateBufferView(VkFormat format,
        VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE);

    VkDescriptorBufferInfo GetDescriptorInfo(VkDeviceSize offset, VkDeviceSize range) const
    {
        VkDescriptorBufferInfo info = {};
        info.buffer = m_handle;
        info.offset = offset;
        info.range = range;
        return info;
    }

    VkDescriptorBufferInfo GetDescriptorInfo() const
    {
        return GetDescriptorInfo(0, GetSize());
    }

    // The memory must be host visible.
    void Read(void* dest, VkDeviceSize offset, VkDeviceSize size);
    void Read(void* dest);
    void Write(const void* data, VkDeviceSize offset, VkDeviceSize size);
    void Write(const void* data);

private:
    rad::Ref<VulkanDevice>      m_device;
    VkBuffer                    m_handle = VK_NULL_HANDLE;
    VkDeviceSize                m_size = 0;
    VkBufferUsageFlags          m_usage;
    VkSharingMode               m_sharingMode;
    VmaAllocation               m_allocation;
    VmaAllocationInfo           m_allocationInfo;
    VkMemoryPropertyFlags       m_memoryFlags;

}; // class VulkanBuffer

class VulkanBufferView : public VulkanObject
{
public:
    VulkanBufferView(rad::Ref<VulkanDevice> device, rad::Ref<VulkanBuffer> buffer,
        const VkBufferViewCreateInfo& createInfo);
    ~VulkanBufferView();

    VkBufferView GetHandle() const { return m_handle; }

private:
    rad::Ref<VulkanDevice>      m_device;
    rad::Ref<VulkanBuffer>      m_buffer;
    VkBufferView                m_handle = VK_NULL_HANDLE;
    VkFormat                    m_format = VK_FORMAT_UNDEFINED;
    VkDeviceSize                m_offset = 0;
    VkDeviceSize                m_range = 0;

}; // class VulkanBufferView

} // namespace rad
