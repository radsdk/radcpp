#include "WindowTest.h"

WindowTest::WindowTest()
{
    m_logger = rad::CreateLogger("WindowTest");
    RAD_LOG(m_logger, info, __func__);
}

WindowTest::~WindowTest()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnShown()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnHidden()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnExposed()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnMoved(int x, int y)
{
    RAD_LOG(m_logger, info, "OnMoved: x={:4}; y={:4};", x, y);
}

void WindowTest::OnResized(int width, int height)
{
    RAD_LOG(m_logger, info, "OnResized: width={:4}; height={:4};", width, height);
}

void WindowTest::OnPixelSizeChanged(int width, int height)
{
    RAD_LOG(m_logger, info, "OnPixelSizeChanged: width={:4}; height={:4};", width, height);
}

void WindowTest::OnMinimized()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnMaximized()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnRestored()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnMouseEnter()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnMouseLeave()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnFocusGained()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnFocusLost()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnCloseRequested()
{
    RAD_LOG(m_logger, info, __func__);
    Destroy();
}

void WindowTest::OnTakeFocus()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnHitTest()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnIccProfileChanged()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnDisplayChanged()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnDisplayScaleChanged()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnOccluded()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnEnterFullscreen()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnLeaveFullscreen()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnDestroyed()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnPenEnter()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnPenLeave()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnKeyDown(const SDL_KeyboardEvent& keyDown)
{
    RAD_LOG(m_logger, info, "OnKeyDown: {}", SDL_GetKeyName(keyDown.keysym.sym));
}

void WindowTest::OnKeyUp(const SDL_KeyboardEvent& keyUp)
{
    RAD_LOG(m_logger, info, "OnKeyUp: {}", SDL_GetKeyName(keyUp.keysym.sym));
}

void WindowTest::OnTextEditing(const SDL_TextEditingEvent& textEditing)
{
    RAD_LOG(m_logger, info, "OnTextEditing: {}", textEditing.text);
}

void WindowTest::OnTextInput(const SDL_TextInputEvent& textInput)
{
    RAD_LOG(m_logger, info, "OnTextInput: {}", textInput.text);
}

void WindowTest::OnMouseMove(const SDL_MouseMotionEvent& mouseMotion)
{
    RAD_LOG(m_logger, info, "OnMouseMove: x={:4} ({:+4}); y={:4} ({:+4})",
        mouseMotion.x, mouseMotion.xrel, mouseMotion.y, mouseMotion.yrel);
}

const char* GetMouseButtonName(Uint8 button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT: return "SDL_BUTTON_LEFT";
    case SDL_BUTTON_MIDDLE: return "SDL_BUTTON_MIDDLE";
    case SDL_BUTTON_RIGHT: return "SDL_BUTTON_RIGHT";
    case SDL_BUTTON_X1: return "SDL_BUTTON_X1";
    case SDL_BUTTON_X2: return "SDL_BUTTON_X2";
    }
    return "SDL_BUTTON_UNKNOWN";
}

void WindowTest::OnMouseButtonDown(const SDL_MouseButtonEvent& mouseButton)
{
    RAD_LOG(m_logger, info, "OnMouseButtonDown: {}", GetMouseButtonName(mouseButton.button));
}

void WindowTest::OnMouseButtonUp(const SDL_MouseButtonEvent& mouseButton)
{
    RAD_LOG(m_logger, info, "OnMouseButtonUp: {}", GetMouseButtonName(mouseButton.button));
}

void WindowTest::OnMouseWheel(const SDL_MouseWheelEvent& mouseWheel)
{
    RAD_LOG(m_logger, info, "OnMouseWheel: x={:+2}; y={:+2}",
        mouseWheel.x, mouseWheel.y);
}
