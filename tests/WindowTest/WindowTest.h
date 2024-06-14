#pragma once

#include <rad/Gui/Window.h>

class WindowTest : public rad::Window
{
public:
    WindowTest();
    ~WindowTest();

protected:
    virtual void OnShown() override;
    virtual void OnHidden() override;
    virtual void OnExposed() override;
    virtual void OnMoved(int x, int y) override;
    virtual void OnResized(int width, int height) override;
    virtual void OnPixelSizeChanged(int width, int height) override;
    virtual void OnMinimized() override;
    virtual void OnMaximized() override;
    virtual void OnRestored() override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseLeave() override;
    // Window has gained keyboard focus.
    virtual void OnFocusGained() override;
    // Window has lost keyboard focus.
    virtual void OnFocusLost() override;
    virtual void OnCloseRequested() override;
    virtual void OnTakeFocus() override;
    virtual void OnHitTest() override;
    virtual void OnIccProfileChanged() override;
    virtual void OnDisplayChanged() override;
    virtual void OnDisplayScaleChanged() override;
    virtual void OnOccluded() override;
    virtual void OnEnterFullscreen() override;
    virtual void OnLeaveFullscreen() override;
    virtual void OnDestroyed() override;
    virtual void OnPenEnter() override;
    virtual void OnPenLeave() override;

private:
    std::shared_ptr<spdlog::logger> m_logger;

}; // class WindowTest
