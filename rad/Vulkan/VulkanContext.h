#pragma once

#include <rad/Vulkan/VulkanInstance.h>
#include <rad/Vulkan/VulkanPhysicalDevice.h>
#include <rad/Vulkan/VulkanDevice.h>
#include <rad/Vulkan/VulkanQueue.h>
#include <rad/Vulkan/VulkanCommand.h>
#include <rad/Vulkan/VulkanFence.h>
#include <rad/Vulkan/VulkanSemaphore.h>
#include <rad/Vulkan/VulkanEvent.h>
#include <rad/Vulkan/VulkanRenderPass.h>
#include <rad/Vulkan/VulkanFramebuffer.h>
#include <rad/Vulkan/VulkanPipeline.h>
#include <rad/Vulkan/VulkanBuffer.h>
#include <rad/Vulkan/VulkanImage.h>
#include <rad/Vulkan/VulkanSampler.h>
#include <rad/Vulkan/VulkanDescriptor.h>
#include <rad/Vulkan/VulkanSurface.h>
#include <rad/Vulkan/VulkanSwapchain.h>

namespace rad
{

class VulkanContext : public RefCounted<VulkanContext>
{
public:
    VulkanContext();
    ~VulkanContext();

    bool Init(std::string_view appName, uint32_t appVersion);

    VulkanInstance* GetInstance() { return m_instance.get(); }
    VulkanDevice* GetDevice() { return m_device.get(); }
    VulkanQueue* GetQueue(VulkanQueueFamily queueFamily = VulkanQueueFamilyUniversal);

    void WaitIdle() { m_device->WaitIdle(); }
    void WaitIdle(VulkanQueueFamily queueFamily) { GetQueue(queueFamily)->WaitIdle(); }

    void ReadBuffer(VulkanBuffer* buffer, void* dest, VkDeviceSize offset, VkDeviceSize size);
    void ReadBuffer(VulkanBuffer* buffer, void* dest);
    void WriteBuffer(VulkanBuffer* buffer, const void* data, VkDeviceSize offset, VkDeviceSize size);
    void WriteBuffer(VulkanBuffer* buffer, const void* data);

private:
    bool CreateInstance(std::string_view appName, uint32_t appVersion);
    bool CreateDevice(VulkanPhysicalDevice* physicalDevice = nullptr);
    bool CreateQueues();

    Ref<VulkanInstance> m_instance;
    std::vector<Ref<VulkanPhysicalDevice>> m_physicalDevices;
    Ref<VulkanDevice> m_device;
    rad::Ref<VulkanQueue> m_queues[VulkanQueueFamilyCount];

}; // class VulkanContext

} // namespace rad
