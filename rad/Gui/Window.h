#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/Integer.h>
#include <rad/Core/String.h>
#include <rad/Core/RefCounted.h>
#include <rad/IO/Logging.h>
#include <rad/Gui/EventHandler.h>
#include <SDL3/SDL.h>

namespace rad
{

class Window : public EventHandler
{
public:
    Window();
    ~Window();

    bool Create(const char* title, int w, int h, SDL_WindowFlags flags);
    void Destroy();

    SDL_Window* GetHandle() { return m_handle; }
    SDL_WindowID GetID() const { return m_id; }

protected:
    SDL_Window* m_handle = nullptr;
    SDL_WindowID m_id = 0;
    // Customized event handlers.
    std::vector<EventHandler*> m_eventHandlers;

    virtual bool OnEvent(const SDL_Event& event) override;
    virtual void OnIdle() override;

    // Window events:
    virtual void OnWindowEvent(const SDL_WindowEvent& event);
    virtual void OnShown() {}
    virtual void OnHidden() {}
    virtual void OnExposed() {}
    virtual void OnMoved(int x, int y) {}
    virtual void OnResized(int width, int height) {}
    virtual void OnPixelSizeChanged(int width, int height) {}
    virtual void OnMinimized() {}
    virtual void OnMaximized() {}
    virtual void OnRestored() {}
    virtual void OnMouseEnter() {}
    virtual void OnMouseLeave() {}
    // Window has gained keyboard focus.
    virtual void OnFocusGained() {}
    // Window has lost keyboard focus.
    virtual void OnFocusLost() {}
    virtual void OnCloseRequested() { Destroy(); }
    virtual void OnTakeFocus() {}
    virtual void OnHitTest() {}
    virtual void OnIccProfileChanged() {}
    virtual void OnDisplayChanged() {}
    virtual void OnDisplayScaleChanged() {}
    virtual void OnOccluded() {}
    virtual void OnEnterFullscreen() {}
    virtual void OnLeaveFullscreen() {}
    virtual void OnDestroyed() {}
    virtual void OnPenEnter() {}
    virtual void OnPenLeave() {}

    // Keyboard events:
    virtual void OnKeyDown(const SDL_KeyboardEvent& keyDown) {}
    virtual void OnKeyUp(const SDL_KeyboardEvent& keyUp) {}
    virtual void OnTextEditing(const SDL_TextEditingEvent& textEditing) {}
    virtual void OnTextInput(const SDL_TextInputEvent& textInput) {}

    // Mouse events:
    virtual void OnMouseMove(const SDL_MouseMotionEvent& mouseMotion) {}
    virtual void OnMouseButtonDown(const SDL_MouseButtonEvent& mouseButton) {}
    virtual void OnMouseButtonUp(const SDL_MouseButtonEvent& mouseButton) {}
    virtual void OnMouseWheel(const SDL_MouseWheelEvent& mouseWheel) {}

}; // class Window

} // namespace rad
