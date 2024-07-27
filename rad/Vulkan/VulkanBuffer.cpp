#include <rad/Vulkan/VulkanBuffer.h>
#include <rad/Vulkan/VulkanDevice.h>

namespace rad
{

VulkanBuffer::VulkanBuffer(
    rad::Ref<VulkanDevice> device,
    const VkBufferCreateInfo& createInfo,
    const VmaAllocationCreateInfo& allocInfo) :
    m_device(std::move(device))
{
    VK_CHECK(vmaCreateBuffer(m_device->GetAllocator(), &createInfo, &allocInfo,
        &m_handle, &m_allocation, &m_allocationInfo));

    m_size = createInfo.size;
    m_usage = createInfo.usage;
    m_sharingMode = createInfo.sharingMode;

    vmaGetMemoryTypeProperties(
        m_device->GetAllocator(), m_allocationInfo.memoryType, &m_memoryFlags);
}

VulkanBuffer::~VulkanBuffer()
{
    vmaDestroyBuffer(m_device->GetAllocator(), m_handle, m_allocation);
}

bool VulkanBuffer::IsHostVisible() const
{
    return HasBits<uint32_t>(m_memoryFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

bool VulkanBuffer::IsHostCoherent() const
{
    return HasBits<uint32_t>(m_memoryFlags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

VkDeviceAddress VulkanBuffer::GetDeviceAddress() const
{
    VkBufferDeviceAddressInfo deviceAddressInfo = {};
    deviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
    deviceAddressInfo.pNext = nullptr;
    deviceAddressInfo.buffer = m_handle;
    return vkGetBufferDeviceAddress(m_device->GetHandle(), &deviceAddressInfo);
}

void* VulkanBuffer::GetMappedAddr()
{
    return m_allocationInfo.pMappedData;
}

void* VulkanBuffer::MapMemory(VkDeviceSize offset, VkDeviceSize size)
{
    if (IsHostVisible())
    {
        void* pMappedAddr = nullptr;
        VK_CHECK(vmaMapMemory(m_device->GetAllocator(), m_allocation, &pMappedAddr));
        return pMappedAddr;
    }
    else
    {
        return nullptr;
    }
}

void* VulkanBuffer::MapMemory()
{
    return MapMemory(0, m_size);
}

void VulkanBuffer::UnmapMemory()
{
    vmaUnmapMemory(m_device->GetAllocator(), m_allocation);
}

void VulkanBuffer::FlushAllocation(VkDeviceSize offset, VkDeviceSize size)
{
    VK_CHECK(vmaFlushAllocation(m_device->GetAllocator(), m_allocation, offset, size));
}

void VulkanBuffer::FlushAllocation()
{
    VK_CHECK(vmaFlushAllocation(m_device->GetAllocator(), m_allocation, 0, m_size));
}

void VulkanBuffer::InvalidateAllocation(VkDeviceSize offset, VkDeviceSize size)
{
    VK_CHECK(vmaInvalidateAllocation(m_device->GetAllocator(), m_allocation, offset, size));
}

void VulkanBuffer::InvalidateAllocation()
{
    VK_CHECK(vmaInvalidateAllocation(m_device->GetAllocator(), m_allocation, 0, m_size));
}

rad::Ref<VulkanBufferView> VulkanBuffer::CreateBufferView(VkFormat format, VkDeviceSize offset, VkDeviceSize range)
{
    VkBufferViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.buffer = m_handle;
    createInfo.format = format;
    createInfo.offset = offset;
    createInfo.range = range;

    return RAD_NEW VulkanBufferView(m_device, this, createInfo);
}

void VulkanBuffer::Read(void* dest, VkDeviceSize offset, VkDeviceSize size)
{
    assert(IsHostVisible());
    void* pMappedAddr = MapMemory(offset, size);
    if (pMappedAddr)
    {
        if (!IsHostCoherent())
        {
            InvalidateAllocation(offset, size);
        }
        memcpy(dest, pMappedAddr, size);
        UnmapMemory();
    }
}

void VulkanBuffer::Read(void* dest)
{
    Read(dest, 0, m_size);
}

void VulkanBuffer::Write(const void* data, VkDeviceSize offset, VkDeviceSize size)
{
    assert(IsHostVisible());
    void* pMappedAddr = MapMemory(offset, size);
    if (pMappedAddr)
    {
        memcpy(pMappedAddr, data, size);
        if (!IsHostCoherent())
        {
            FlushAllocation(offset, size);
        }
        UnmapMemory();
    }
}

void VulkanBuffer::Write(const void* data)
{
    Write(data, 0, m_size);
}

VulkanBufferView::VulkanBufferView(
    rad::Ref<VulkanDevice> device,
    rad::Ref<VulkanBuffer> buffer,
    const VkBufferViewCreateInfo& createInfo) :
    m_device(std::move(device)),
    m_buffer(std::move(buffer))
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateBufferView(m_device->GetHandle(), &createInfo, nullptr, &m_handle));
    m_format = createInfo.format;
    m_offset = createInfo.offset;
    m_range = createInfo.range;
}

VulkanBufferView::~VulkanBufferView()
{
    m_device->GetFunctionTable()->
        vkDestroyBufferView(m_device->GetHandle(), m_handle, nullptr);
    m_handle = VK_NULL_HANDLE;
}

} // namespace rad
