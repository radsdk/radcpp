#ifndef VULKAN_SWAPCHAIN_H
#define VULKAN_SWAPCHAIN_H

#include "VulkanCommon.h"

class VulkanSwapchain : public VulkanObject
{
public:
    VulkanSwapchain(rad::Ref<VulkanDevice> device, const VkSwapchainCreateInfoKHR& createInfo);
    ~VulkanSwapchain();

    VkSwapchainKHR GetHandle() const { return m_handle; }

    uint32_t GetImageCount();
    VkFormat GetFormat();
    uint32_t GetWidth();
    uint32_t GetHeight();

    VulkanImage* GetImage(uint32_t index);
    VulkanImageView* GetDefaultView(uint32_t index);
    VulkanImage* GetCurrentImage();

    uint32_t GetCurrentImageIndex() const;
    // @param timeout: indicates how long the function waits, in nanoseconds, if no image is available.
    uint32_t AcquireNextImage(
        VulkanSemaphore* semaphore, VulkanFence* fence = nullptr, uint64_t timeout = UINT64_MAX);

private:
    rad::Ref<VulkanDevice>      m_device;
    VkSwapchainKHR              m_handle = VK_NULL_HANDLE;
    VkFormat                    m_format;
    uint32_t                    m_imageCount;
    uint32_t                    m_width;
    uint32_t                    m_height;
    std::vector<rad::Ref<VulkanImage>> m_images;
    uint32_t                    m_currentImageIndex = 0;

}; // class VulkanSwapchain

#endif // VULKAN_SWAPCHAIN_H
