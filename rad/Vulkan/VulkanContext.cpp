#include <rad/Vulkan/VulkanContext.h>

namespace rad
{

VulkanContext::VulkanContext()
{
}

VulkanContext::~VulkanContext()
{
}

bool VulkanContext::Init(std::string_view appName, uint32_t appVersion)
{
    if (!CreateInstance(appName, appVersion))
    {
        return false;
    }
    if (!CreateDevice())
    {
        return false;
    }
    if (!CreateQueues())
    {
        return false;
    }
    return true;
}

bool VulkanContext::CreateInstance(std::string_view appName, uint32_t appVersion)
{
    m_instance = RAD_NEW VulkanInstance();
    if (m_instance->Init(appName, appVersion))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool VulkanContext::CreateDevice(VulkanPhysicalDevice* physicalDevice)
{
    if (m_physicalDevices.empty())
    {
        m_physicalDevices = m_instance->EnumeratePhysicalDevices();
    }

    for (size_t i = 0; i < m_physicalDevices.size(); ++i)
    {
        const VkPhysicalDeviceProperties& properties = m_physicalDevices[i]->m_properties;
        RAD_LOG(GetVulkanLogger(), info, "PhysicalDevice#{}: {} (0x{:04X})",
            i, properties.deviceName, properties.deviceID);
    }

    if (!physicalDevice)
    {
        for (size_t i = 0; i < m_physicalDevices.size(); ++i)
        {
            const VkPhysicalDeviceProperties& properties = m_physicalDevices[i]->m_properties;
            // Prefer the first discrete GPU.
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                physicalDevice = m_physicalDevices[i].get();
                break;
            }
        }
    }

    std::set<std::string> extensionNames;
    for (const VkExtensionProperties& extension : physicalDevice->m_extensions)
    {
        if (std::string_view(extension.extensionName).starts_with("VK_KHR") ||
            std::string_view(extension.extensionName).starts_with("VK_EXT"))
        {
            extensionNames.insert(extension.extensionName);
        }
    }

    if (!m_instance->IsExtensionSupported("VK_KHR_get_physical_device_properties2") ||
        !m_instance->IsExtensionSupported("VK_KHR_surface") ||
        !m_instance->IsExtensionSupported("VK_KHR_get_surface_capabilities2") ||
        !m_instance->IsExtensionSupported("VK_KHR_swapchain"))
    {
        extensionNames.erase("VK_EXT_full_screen_exclusive");
    }

    if (extensionNames.find("VK_EXT_surface_maintenance1") == extensionNames.end())
    {
        extensionNames.erase("VK_EXT_swapchain_maintenance1");
    }

    if (extensionNames.find("VK_KHR_buffer_device_address") != extensionNames.end())
    {
        extensionNames.erase("VK_EXT_buffer_device_address");
    }

    m_device = physicalDevice->CreateDevice(extensionNames);
    RAD_LOG(GetVulkanLogger(), info, "Vulkan logical device created: {} (0x{:04X})",
        physicalDevice->m_properties.deviceName,
        physicalDevice->m_properties.deviceID);

    return m_device.get();
}

bool VulkanContext::CreateQueues()
{
    for (uint32_t i = 0; i < VulkanQueueFamilyCount; ++i)
    {
        VulkanQueueFamily queueFamily = VulkanQueueFamily(i);
        if (m_device->IsQueueFamilySupported(queueFamily))
        {
            m_queues[i] = m_device->CreateQueue(VulkanQueueFamily(queueFamily));
        }
    }

    return m_queues[VulkanQueueFamilyUniversal].get();
}

VulkanQueue* VulkanContext::GetQueue(VulkanQueueFamily queueFamily)
{
    return m_queues[queueFamily].get();
}

void VulkanContext::ReadBuffer(
    VulkanBuffer* buffer, void* dest, VkDeviceSize offset, VkDeviceSize size)
{
    if (buffer->IsHostVisible())
    {
        buffer->Read(dest);
    }
    else
    {
        rad::Ref<VulkanBuffer> stagingBuffer = m_device->CreateStagingBuffer(size);

        VulkanQueueFamily queueFamily = VulkanQueueFamilyUniversal;
        Ref<VulkanCommandPool> cmdPool =
            m_device->CreateCommandPool(queueFamily, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
        rad::Ref<VulkanCommandBuffer> cmdBuffer = cmdPool->Allocate();
        cmdBuffer->CreateCheckpoint();

        VkBufferMemoryBarrier srcBarrier = {};
        srcBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        srcBarrier.pNext;
        srcBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
        srcBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        srcBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        srcBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        srcBarrier.buffer = buffer->GetHandle();
        srcBarrier.offset = offset;
        srcBarrier.size = size;

        VkBufferCopy copyRegion = {};
        copyRegion.srcOffset = offset;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;
        cmdBuffer->SetPipelineBarrier(
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            {}, srcBarrier, {});
        cmdBuffer->CopyBuffer(buffer, stagingBuffer.get(), copyRegion);
        VkBufferMemoryBarrier hostReadBarrier = {};
        hostReadBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        hostReadBarrier.pNext;
        hostReadBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        hostReadBarrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
        hostReadBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        hostReadBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        hostReadBarrier.buffer = stagingBuffer->GetHandle();
        hostReadBarrier.offset = 0;
        hostReadBarrier.size = size;
        cmdBuffer->SetPipelineBarrier(
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_HOST_BIT,
            0,
            {}, hostReadBarrier, {});
        cmdBuffer->End();
        GetQueue(queueFamily)->SubmitAndWait(cmdBuffer.get());
        stagingBuffer->Read(dest);
    }
}

void VulkanContext::ReadBuffer(VulkanBuffer* buffer, void* dest)
{
    ReadBuffer(buffer, dest, 0, buffer->GetSize());
}

void VulkanContext::WriteBuffer(
    VulkanBuffer* buffer, const void* data, VkDeviceSize offset, VkDeviceSize size)
{
    if (buffer->IsHostVisible())
    {
        buffer->Write(data, offset, size);
    }
    else
    {
        VulkanQueueFamily queueFamily = VulkanQueueFamilyUniversal;
        Ref<VulkanCommandPool> cmdPool =
            m_device->CreateCommandPool(queueFamily, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
        rad::Ref<VulkanBuffer> stagingBuffer = m_device->CreateStagingBuffer(size);
        stagingBuffer->Write(data);

        rad::Ref<VulkanCommandBuffer> cmdBuffer = cmdPool->Allocate();
        cmdBuffer->CreateCheckpoint();
        VkBufferCopy copyRegion = {};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = offset;
        copyRegion.size = size;
        cmdBuffer->CopyBuffer(stagingBuffer.get(), buffer, copyRegion);
        cmdBuffer->End();
        GetQueue(queueFamily)->SubmitAndWait(cmdBuffer.get());
    }
}

void VulkanContext::WriteBuffer(VulkanBuffer* buffer, const void* data)
{
    WriteBuffer(buffer, data, 0, buffer->GetSize());
}

} // namespace rad
