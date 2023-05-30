#pragma once

#include "VulkanEngine/Gui/VulkanWindow.h"
#include "VulkanEngine/Gui/VulkanFrame.h"
#include "VulkanEngine/Gui/VulkanGuiContext.h"


class VulkanViewer : public VulkanWindow
{
public:
    VulkanViewer();
    ~VulkanViewer();

    bool Init();

    virtual bool OnEvent(const SDL_Event& event);

    virtual void OnShown() override;
    virtual void OnHidden() override;
    virtual void OnExposed() override;
    virtual void OnMoved(int x, int y) override;
    virtual void OnResized(int width, int height) override;
    virtual void OnMinimized() override;
    virtual void OnMaximized() override;
    virtual void OnRestored() override;
    virtual void OnEnter() override;
    virtual void OnLeave() override;
    virtual void OnKeyDown(const SDL_KeyboardEvent& keyDown) override;
    virtual void OnKeyUp(const SDL_KeyboardEvent& keyUp) override;
    virtual void OnMouseMove(const SDL_MouseMotionEvent& mouseMotion) override;
    virtual void OnMouseButtonDown(const SDL_MouseButtonEvent& mouseButton) override;
    virtual void OnMouseButtonUp(const SDL_MouseButtonEvent& mouseButton) override;
    virtual void OnMouseWheel(const SDL_MouseWheelEvent& mouseWheel) override;

    virtual void OnRender() override;

private:
    const char* GetMouseButtonName(Uint8 button);

    rad::Ref<VulkanFrame> m_frame;
    rad::Ref<VulkanGuiContext> m_gui;
    rad::Ref<VulkanImage> m_renderTarget;
    rad::Ref<VulkanImage> m_depthStencil;

}; // class VulkanViewer
