#pragma once

#include "VulkanWindow.h"

class VulkanFrame : public rad::RefCounted<VulkanFrame>
{
public:
    VulkanFrame(VulkanWindow* window);
    ~VulkanFrame();

    VulkanSwapchain* GetSwapchain() const { return m_swapchain.get(); }

    void Resize(uint32_t width, uint32_t height);

    void SetColorBuffer(rad::Ref<VulkanImage> colorBuffer);
    void SetOverlay(rad::Ref<VulkanImage> overlay);

    uint32_t BeginFrame();
    void EndFrame();

private:
    void RenderToSwapchain();
    void Present();

    VkSurfaceFormatKHR SelectSurfaceFormat(rad::Span<VkSurfaceFormatKHR> candidateFormats);
    bool CreateSwapchain(uint32_t width, uint32_t height);
    bool CreateRenderPass();
    bool CreateFramebuffers(uint32_t width, uint32_t height);
    bool CreateSampler();
    bool CreatePipeline();

    VulkanWindow* m_window;
    VulkanDevice* m_device = nullptr;

    VkSurfaceCapabilitiesKHR m_surfaceCapabilities = {};
    std::vector<VkSurfaceFormatKHR> m_surfaceFormats;
    std::vector<VkPresentModeKHR> m_presentModes;

    rad::Ref<VulkanCommandPool> m_commandPool;
    std::vector<rad::Ref<VulkanCommandBuffer>> m_commandBuffers;

    rad::Ref<VulkanSwapchain> m_swapchain;
    VkSurfaceFormatKHR m_surfaceFormat;
    uint32_t m_swapchainImageCount = 3;
    uint32_t m_currentBufferIndex = 0;

    // Max frame count in flight/ Allow a maximum of two outstanding presentation operations.
    static constexpr uint32_t FrameLag = 2;
    rad::Ref<VulkanSemaphore> m_swapchainImageAcquired[FrameLag];
    rad::Ref<VulkanSemaphore> m_drawComplete[FrameLag];
    rad::Ref<VulkanSemaphore> m_swapchainImageOwnershipTransferComplete[FrameLag];
    // Fences that we can use to throttle if we get too far ahead of image presents.
    rad::Ref<VulkanFence> m_frameThrottles[FrameLag];
    uint32_t m_frameIndex = 0;

    // Render to swapchain

    rad::Ref<VulkanBuffer> m_vertexBuffer;
    rad::Ref<VulkanSampler> m_sampler;

    rad::Ref<VulkanRenderPass> m_renderPass;
    std::vector<rad::Ref<VulkanFramebuffer>> m_framebuffers;

    rad::Ref<VulkanShader> m_shaderVert;
    rad::Ref<VulkanShader> m_shaderFrag;

    rad::Ref<VulkanDescriptorPool> m_descriptorPool;
    rad::Ref<VulkanDescriptorSetLayout> m_descriptorSetLayout;
    rad::Ref<VulkanDescriptorSet> m_descriptorSet;
    rad::Ref<VulkanPipelineLayout> m_pipelineLayout;
    rad::Ref<VulkanPipeline> m_pipeline;

    rad::Ref<VulkanImage> m_colorBuffer;
    rad::Ref<VulkanImage> m_overlay;

}; // class VulkanFrame
