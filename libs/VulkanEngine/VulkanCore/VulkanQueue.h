#ifndef VULKAN_QUEUE_H
#define VULKAN_QUEUE_H

#include "VulkanCommon.h"

struct VulkanSubmitWait
{
    VulkanSemaphore* semaphore;
    VkPipelineStageFlags dstStageMask;
};

class VulkanQueue : public VulkanObject
{
public:
    VulkanQueue(rad::Ref<VulkanDevice> device, VulkanQueueFamily queueFamily);
    ~VulkanQueue();

    VkQueue GetHandle() const { return m_handle; }

    VulkanQueueFamily GetQueueFamily() const { return m_queueFamily; }
    const VkQueueFamilyProperties& GetQueueFamilyProperties() const;

    void Submit(
        rad::Span<VulkanCommandBuffer*> commandBuffers,
        rad::Span<VulkanSubmitWait>     waitSemaphores = {},
        rad::Span<VulkanSemaphore*>     signalSemaphores = {},
        VulkanFence* fence = nullptr
    );

    // Create a fence implicitly; wait GPU to complete the commands and notify the host.
    void SubmitAndWaitForCompletion(
        rad::Span<VulkanCommandBuffer*> commandBuffers,
        rad::Span<VulkanSubmitWait>     waitSemaphores = {},
        rad::Span<VulkanSemaphore*>     signalSemaphores = {}
    );

    void WaitIdle();

    bool Present(
        rad::Span<VulkanSemaphore*>     waitSemaphores,
        rad::Span<VulkanSwapchain*>     swapchains,
        rad::Span<uint32_t>             imageIndices);
    bool Present(
        rad::Span<VulkanSemaphore*>     waitSemaphores,
        rad::Span<VulkanSwapchain*>     swapchains);

private:
    rad::Ref<VulkanDevice>      m_device;
    VkQueue                     m_handle = VK_NULL_HANDLE;
    VulkanQueueFamily           m_queueFamily;

}; // class VulkanQueue

#endif // VULKAN_QUEUE_H
