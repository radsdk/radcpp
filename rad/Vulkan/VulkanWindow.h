#pragma once

#include <rad/Vulkan/VulkanContext.h>
#include <rad/Gui/Window.h>

namespace rad
{

class VulkanWindow : public Window
{
public:
    VulkanWindow();
    VulkanWindow(Ref<VulkanContext> context);
    ~VulkanWindow();

    void SetVulkanContext(Ref<VulkanContext> context);

protected:
    rad::Ref<VulkanSurface> CreateSurface();

    virtual bool OnEvent(const SDL_Event& event) override;
    virtual void OnResized(int width, int height) override;
    virtual void OnDestroyed() override;

    Ref<VulkanContext> m_context;

}; // class VulkanWindow

} // namespace rad
