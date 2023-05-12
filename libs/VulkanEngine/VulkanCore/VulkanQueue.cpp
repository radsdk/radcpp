#include "VulkanQueue.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFence.h"
#include "VulkanEvent.h"
#include "VulkanSemaphore.h"
#include "VulkanSwapchain.h"

#include "boost/container/small_vector.hpp"

VulkanQueue::VulkanQueue(rad::Ref<VulkanDevice> device, VulkanQueueFamily queueFamily) :
    m_device(std::move(device)),
    m_queueFamily(queueFamily)
{
    m_device->GetFunctionTable()->
        vkGetDeviceQueue(m_device->GetHandle(), m_device->GetQueueFamilyIndex(queueFamily), 0, &m_handle);
}

VulkanQueue::~VulkanQueue()
{
    m_handle = VK_NULL_HANDLE;
}

const VkQueueFamilyProperties& VulkanQueue::GetQueueFamilyProperties() const
{
    uint32_t queueFamilyIndex = m_device->GetQueueFamilyIndex(m_queueFamily);
    return m_device->GetPhysicalDevice()->GetQueueFamilyProperties()[queueFamilyIndex];
}

void VulkanQueue::Submit(
    rad::Span<VulkanCommandBuffer*>      commandBuffers,
    rad::Span<VulkanSubmitWait>          waitSemaphores,
    rad::Span<VulkanSemaphore*>          signalSemaphores,
    VulkanFence* fence)
{
    boost::container::small_vector<VkCommandBuffer, 8> commandBuffersHandles(commandBuffers.size());
    for (int i = 0; i < commandBuffers.size(); i++)
    {
        commandBuffersHandles[i] = commandBuffers[i]->GetHandle();
    }

    boost::container::small_vector<VkSemaphore, 8> waitSemaphoresHandles(waitSemaphores.size());
    boost::container::small_vector<VkPipelineStageFlags, 8> waitDstStageMasks(waitSemaphores.size());
    for (int i = 0; i < waitSemaphores.size(); i++)
    {
        waitSemaphoresHandles[i] = waitSemaphores[i].semaphore->GetHandle();
        waitDstStageMasks[i] = waitSemaphores[i].dstStageMask;
    }

    boost::container::small_vector<VkSemaphore, 8> signalSemaphoresHandles(signalSemaphores.size());
    for (int i = 0; i < signalSemaphores.size(); i++)
    {
        signalSemaphoresHandles[i] = signalSemaphores[i]->GetHandle();
    }

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphoresHandles.size());
    submitInfo.pWaitSemaphores = waitSemaphoresHandles.data();
    submitInfo.pWaitDstStageMask = waitDstStageMasks.data();
    submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffersHandles.size());
    submitInfo.pCommandBuffers = commandBuffersHandles.data();
    submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphoresHandles.size());
    submitInfo.pSignalSemaphores = signalSemaphoresHandles.data();

    VK_CHECK(m_device->GetFunctionTable()->
        vkQueueSubmit(m_handle, 1, &submitInfo, fence ? fence->GetHandle() : VK_NULL_HANDLE));
}

void VulkanQueue::SubmitAndWaitForCompletion(
    rad::Span<VulkanCommandBuffer*>      commandBuffers,
    rad::Span<VulkanSubmitWait>          waitSemaphores,
    rad::Span<VulkanSemaphore*>          signalSemaphores)
{
    rad::Ref<VulkanFence> fence = m_device->CreateFence();
    Submit(commandBuffers, waitSemaphores, signalSemaphores, fence.get());
    fence->Wait();
}

void VulkanQueue::WaitIdle()
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkQueueWaitIdle(m_handle));
}

bool VulkanQueue::Present(
    rad::Span<VulkanSemaphore*> waitSemaphores,
    rad::Span<VulkanSwapchain*> swapchains,
    rad::Span<uint32_t> imageIndices)
{
    assert(swapchains.size() == imageIndices.size());

    boost::container::small_vector<VkResult, 8> results(swapchains.size());

    boost::container::small_vector<VkSemaphore, 8> waitSemaphoresHandles(waitSemaphores.size());
    for (int i = 0; i < waitSemaphores.size(); i++)
    {
        waitSemaphoresHandles[i] = waitSemaphores[i]->GetHandle();
    }

    boost::container::small_vector<VkSwapchainKHR, 8> swapchainsHandles(swapchains.size());
    for (int i = 0; i < swapchains.size(); i++)
    {
        swapchainsHandles[i] = swapchains[i]->GetHandle();
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphoresHandles.size());
    presentInfo.pWaitSemaphores = waitSemaphoresHandles.data();
    presentInfo.swapchainCount = static_cast<uint32_t>(swapchainsHandles.size());
    presentInfo.pSwapchains = swapchainsHandles.data();
    presentInfo.pImageIndices = imageIndices.data();
    presentInfo.pResults = results.data();

    VK_CHECK(m_device->GetFunctionTable()->
        vkQueuePresentKHR(m_handle, &presentInfo));

    for (VkResult result : results)
    {
        if (result < 0)
        {
            return false;
        }
    }
    return true;
}

bool VulkanQueue::Present(
    rad::Span<VulkanSemaphore*> waitSemaphores,
    rad::Span<VulkanSwapchain*> swapchains)
{
    boost::container::small_vector<uint32_t, 8> imageIndices;
    imageIndices.resize(swapchains.size());
    for (int i = 0; i < swapchains.size(); i++)
    {
        imageIndices[i] = swapchains[i]->GetCurrentImageIndex();
    }
    return Present(waitSemaphores, swapchains, imageIndices);
}
