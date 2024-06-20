#include <rad/Gui/Window.h>
#include <rad/Gui/Application.h>

namespace rad
{

Window::Window()
{
}

Window::~Window()
{
    Destroy();
}

bool Window::Create(const char* title, int w, int h, SDL_WindowFlags flags)
{
    m_handle = SDL_CreateWindow(title, w, h, flags);
    if (m_handle)
    {
        m_id = SDL_GetWindowID(m_handle);
        Application::GetInstance()->RegisterEventHandler(this);
        if (m_id == 0)
        {
            RAD_LOG(GetGuiLogger(), err, "SDL_GetWindowID failed: {}", SDL_GetError());
            return false;
        }
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_CreateWindow failed: {}", SDL_GetError());
        return false;
    }
}

void Window::Destroy()
{
    if (m_handle)
    {
        Application::GetInstance()->UnregisterEventHandler(this);
        SDL_DestroyWindow(m_handle);
        m_handle = nullptr;
    }
}

bool Window::OnEvent(const SDL_Event& event)
{
    for (EventHandler* handler : m_eventHandlers)
    {
        if (handler->OnEvent(event))
        {
            return true;
        }
    }

    if ((event.type >= SDL_EVENT_WINDOW_FIRST) &&
        (event.type <= SDL_EVENT_WINDOW_LAST))
    {
        if (event.window.windowID == m_id)
        {
            OnWindowEvent(event.window);
            return true;
        }
    }
    else if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.windowID == m_id)
        {
            OnKeyDown(event.key);
            return true;
        }
    }
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        if (event.key.windowID == m_id)
        {
            OnKeyUp(event.key);
            return true;
        }
    }
    else if (event.type == SDL_EVENT_TEXT_EDITING)
    {
        if (event.edit.windowID == m_id)
        {
            OnTextEditing(event.edit);
            return true;
        }
    }
    else if (event.type == SDL_EVENT_TEXT_INPUT)
    {
        if (event.text.windowID == m_id)
        {
            OnTextInput(event.text);
            return true;
        }
    }
    else if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        if (event.motion.windowID == m_id)
        {
            OnMouseMove(event.motion);
            return true;
        }
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event.button.windowID == m_id)
        {
            OnMouseButtonDown(event.button);
            return true;
        }
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        if (event.button.windowID == m_id)
        {
            OnMouseButtonUp(event.button);
            return true;
        }
    }
    else if (event.type == SDL_EVENT_MOUSE_WHEEL)
    {
        if (event.wheel.windowID == m_id)
        {
            OnMouseWheel(event.wheel);
            return true;
        }
    }
    else if (event.type == SDL_EVENT_USER)
    {
        if ((event.user.windowID == 0) ||
            (event.user.windowID == m_id))
        {
            OnUserEvent(event.user);
            return (event.user.windowID == m_id);
        }
    }

    return false;
}

void Window::OnIdle()
{
}

void Window::OnWindowEvent(const SDL_WindowEvent& event)
{
    switch (event.type)
    {
    case SDL_EVENT_WINDOW_SHOWN:
        OnShown();
        break;
    case SDL_EVENT_WINDOW_HIDDEN:
        OnHidden();
        break;
    case SDL_EVENT_WINDOW_EXPOSED:
        OnExposed();
        break;
    case SDL_EVENT_WINDOW_MOVED:
        OnMoved(event.data1, event.data2);
        break;
    case SDL_EVENT_WINDOW_RESIZED:
        OnResized(event.data1, event.data2);
        break;
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        OnPixelSizeChanged(event.data1, event.data2);
        break;
    case SDL_EVENT_WINDOW_MINIMIZED:
        OnMinimized();
        break;
    case SDL_EVENT_WINDOW_MAXIMIZED:
        OnMaximized();
        break;
    case SDL_EVENT_WINDOW_RESTORED:
        OnRestored();
        break;
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
        OnMouseEnter();
        break;
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
        OnMouseLeave();
        break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        OnFocusGained();
        break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        OnFocusLost();
        break;
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        OnCloseRequested();
        break;
    case SDL_EVENT_WINDOW_TAKE_FOCUS:
        OnTakeFocus();
        break;
    case SDL_EVENT_WINDOW_HIT_TEST:
        OnHitTest();
        break;
    case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
        OnIccProfileChanged();
        break;
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
        OnDisplayChanged();
        break;
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
        OnDisplayScaleChanged();
        break;
    case SDL_EVENT_WINDOW_OCCLUDED:
        OnOccluded();
        break;
    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        OnEnterFullscreen();
        break;
    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        OnLeaveFullscreen();
        break;
    case SDL_EVENT_WINDOW_DESTROYED:
        OnDestroyed();
        break;
    case SDL_EVENT_WINDOW_PEN_ENTER:
        OnPenEnter();
        break;
    case SDL_EVENT_WINDOW_PEN_LEAVE:
        OnPenLeave();
        break;
    }
}

} // namespace rad
