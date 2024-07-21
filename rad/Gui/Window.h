#pragma once

#include <rad/Gui/Application.h>
#include <rad/Container/Span.h>
#include <SDL3/SDL.h>

namespace rad
{

class Application;
class Surface;

class Window : public EventHandler
{
public:
    Window();
    ~Window();

    bool Create(const char* title, int w, int h, SDL_WindowFlags flags);
    void Destroy();

    SDL_Window* GetHandle() { return m_handle; }
    SDL_WindowID GetID() const { return m_id; }

    float GetPixelDensity();
    float GetDisplayScale();
    bool SetFullscreenMode(const SDL_DisplayMode* mode);
    bool SetFullscreenMode(const SDL_DisplayMode& mode);
    bool SetBorderlessFullscreenDesktopMode();
    const SDL_DisplayMode* GetFullscreenMode();
    // Data returned should be freed with SDL_free.
    const void* GetICCProfile(size_t* size);
    Uint32 GetPixelFormat();

    SDL_WindowFlags GetFlags();
    bool SetTitle(std::string_view title);
    const char* GetTitle();
    bool SetIcon(SDL_Surface* icon);
    bool SetIcon(Surface* icon);
    bool SetPosition(int x, int y);
    bool GetPosition(int* x, int* y);
    bool SetSize(int w, int h);
    bool GetSize(int* w, int* h);
    bool SetAspectRatio(float minAspect, float maxAspect);
    bool GetAspectRatio(float* minAspect, float* maxAspect);
    bool GetSizeInPixels(int* w, int* h);
    bool SetMinimumSize(int w, int h);
    bool GetMinimumSize(int* w, int* h);
    bool SetMaximumSize(int w, int h);
    bool GetMaximumSize(int* w, int* h);
    bool SetBordered(bool bordered);
    bool SetResizable(bool resizable);
    bool SetAlwaysOnTop(bool onTop);

    bool Show();
    bool Hide();
    bool Raise();
    bool Maximize();
    bool Minimize();
    bool Restore();
    bool SetFullscreen(bool fullscreen);
    bool Sync();

    bool HasSurface();
    SDL_Surface* GetSurface();
    bool SetSurfaceVSync(int vsync);
    bool GetSurfaceVSync(int* vsync);
    bool UpdateSurface();
    bool UpdateSurfaceRects(Span<SDL_Rect> rects);
    bool DestroySurface();

    bool SetKeyboardGrab(bool grabbed);
    bool SetMouseGrab(bool grabbed);
    bool GetKeyboardGrab();
    bool GetMouseGrab();

    bool SetMouseRect(const SDL_Rect* rect);
    bool SetMouseRect(const SDL_Rect& rect);
    const SDL_Rect* GetMouseRect();

    bool SetOpacity(float opacity);
    bool GetOpacity(float* opacity);
    bool SetFocusable(bool focusable);

    bool ShowSystemMenu(int x, int y);
    bool SetShape(SDL_Surface* shape);
    bool SetShape(Surface* shape);
    bool Flash(SDL_FlashOperation operation);

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

    // User
    virtual void OnUserEvent(const SDL_UserEvent& user) {}

}; // class Window

} // namespace rad
