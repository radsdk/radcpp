#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

struct VulkanSubmitWaitInfo
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

    VulkanQueueFamily GetQueueFamily() const;
    uint32_t GetQueueFamilyIndex() const;
    const VkQueueFamilyProperties& GetQueueFamilyProperties() const;
    bool SupportGraphics() const;
    bool SupportCompute() const;

    void Submit(
        rad::Span<VulkanCommandBuffer*> commandBuffers,
        rad::Span<VulkanSubmitWaitInfo> waits = {},
        rad::Span<VulkanSemaphore*>     signalSemaphores = {},
        VulkanFence* fence = nullptr
    );

    // Create a fence implicitly; wait the GPU to complete the commands and notify the host.
    void SubmitAndWait(
        rad::Span<VulkanCommandBuffer*> commandBuffers,
        rad::Span<VulkanSubmitWaitInfo> waits = {},
        rad::Span<VulkanSemaphore*>     signalSemaphores = {}
    );

    VkResult WaitIdle();

    VkResult Present(
        rad::Span<VulkanSemaphore*>     waitSemaphores,
        rad::Span<VulkanSwapchain*>     swapchains,
        rad::Span<uint32_t>             imageIndices,
        VkResult* pResults);
    VkResult Present(
        rad::Span<VulkanSemaphore*>     waitSemaphores,
        rad::Span<VulkanSwapchain*>     swapchains);

private:
    rad::Ref<VulkanDevice>      m_device;
    VulkanQueueFamily           m_queueFamily = VulkanQueueFamilyUniversal;
    VkQueue                     m_handle = VK_NULL_HANDLE;

}; // class VulkanQueue

} // namespace rad
