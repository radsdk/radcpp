#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

class VulkanSwapchain : public VulkanObject
{
public:
    VulkanSwapchain(
        rad::Ref<VulkanDevice> device,
        rad::Ref<VulkanSurface> surface,
        const VkSwapchainCreateInfoKHR& createInfo);
    ~VulkanSwapchain();

    VkSwapchainKHR GetHandle() const { return m_handle; }

    uint32_t GetImageCount() const;
    VkFormat GetFormat() const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    VkPresentModeKHR GetPresentMode() const;

    VulkanImage* GetImage(uint32_t index);
    VulkanImageView* GetDefaultView(uint32_t index);
    VulkanImage* GetCurrentImage();
    VulkanImageView* GetCurrentImageView();

    uint32_t GetCurrentImageIndex() const;
    // @param timeout: indicates how long the function waits, in nanoseconds, if no image is available.
    VkResult AcquireNextImage(
        VulkanSemaphore* semaphore, VulkanFence* fence = nullptr, uint64_t timeout = UINT64_MAX);

private:
    rad::Ref<VulkanDevice>      m_device;
    rad::Ref<VulkanSurface>     m_surface;
    VkSwapchainKHR              m_handle = VK_NULL_HANDLE;
    VkFormat                    m_format;
    uint32_t                    m_imageCount;
    uint32_t                    m_width;
    uint32_t                    m_height;
    VkPresentModeKHR            m_presentMode;
    std::vector<rad::Ref<VulkanImage>> m_images;
    std::vector<rad::Ref<VulkanImageView>> m_imageViews;
    uint32_t                    m_currentImageIndex = 0;

}; // class VulkanSwapchain

} // namespace rad
