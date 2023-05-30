#pragma once

#include "VulkanWindow.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_vulkan.h"

class VulkanGuiContext : public rad::RefCounted<VulkanGuiContext>
{
public:
    VulkanGuiContext(VulkanWindow* window);
    ~VulkanGuiContext();

    ImGuiIO& GetIO() { return ImGui::GetIO(); }
    ImFontAtlas* GetFonts() { return ImGui::GetIO().Fonts; }

    struct InitInfo
    {
        uint32_t swapchainImageCount;
        std::vector<VkDescriptorPoolSize> poolSizes;
    };
    bool Init(const InitInfo& guiInfo);
    VulkanImage* GetRenderTarget() { return m_overlay.get(); }

    bool UploadFonts();

    bool ProcessEvent(const SDL_Event& event);
    void StartFrame();
    void Render();
    void Submit();

    void Resize(int width, int height);

private:
    void CreateDescriptorPool(rad::Span<VkDescriptorPoolSize> poolSizes);
    void CreateRenderPass();
    void CreateSizeDependentResources();

    VulkanWindow* m_window;
    VulkanDevice* m_device = nullptr;
    uint32_t m_swapchainImageCount = 0;

    rad::Ref<VulkanCommandPool> m_commandPool;
    std::vector<rad::Ref<VulkanCommandBuffer>> m_commandBuffers;
    uint32_t m_bufferIndex = 0;
    VkClearValue m_clearValue = {};

    rad::Ref<VulkanDescriptorPool> m_descriptorPool;
    rad::Ref<VulkanRenderPass> m_renderPass;

    VkFormat m_colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
    rad::Ref<VulkanImage> m_overlay;
    rad::Ref<VulkanFramebuffer> m_framebuffer;

}; // class VulkanGuiContext
