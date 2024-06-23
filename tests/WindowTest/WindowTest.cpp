#include "WindowTest.h"
#include <rad/IO/Logging.h>
#include <rad/System/Power.h>

WindowTest::WindowTest()
{
    m_logger = rad::CreateLogger("WindowTest");
    RAD_LOG(m_logger, trace, __func__);
}

WindowTest::~WindowTest()
{
    if (m_wave.sound)
    {
        SDL_free(m_wave.sound);
        m_wave.sound = nullptr;
    }
    RAD_LOG(m_logger, trace, __func__);
}

Uint32 PowerWatchdog(void* userData, SDL_TimerID timerID, Uint32 interval)
{
    WindowTest* test = reinterpret_cast<WindowTest*>(userData);
    int seconds = 0;
    int percent = 0;
    SDL_PowerState state = rad::GetPowerInfo(&seconds, &percent);
    if (((state == SDL_POWERSTATE_ON_BATTERY) ||
        (state == SDL_POWERSTATE_CHARGING) ||
        (state == SDL_POWERSTATE_CHARGED)))
    {
        if ((seconds != -1) && (percent != -1))
        {
            RAD_LOG(test->GetLogger(), info, "PowerWatchdog: {}: {} seconds (%{}) remaining.",
                rad::GetPowerStateString(state), seconds, percent);
        }
        else if (seconds != -1)
        {
            RAD_LOG(test->GetLogger(), info, "PowerWatchdog: {}: {} seconds remaining.",
                rad::GetPowerStateString(state), seconds);
        }
        else if (percent != -1)
        {
            RAD_LOG(test->GetLogger(), info, "PowerWatchdog: {}: %{} remaining.",
                rad::GetPowerStateString(state), percent);
        }
        else
        {
            RAD_LOG(test->GetLogger(), info, "PowerWatchdog: {}: life unknown.",
                interval, rad::GetPowerStateString(state));
        }
    }
    else if (state == SDL_POWERSTATE_NO_BATTERY)
    {
        RAD_LOG(test->GetLogger(), info, "PowerWatchdog: {}.",
            rad::GetPowerStateString(state));
    }

    // trigger an user event!
    SDL_Event event = {};
    event.type = SDL_EVENT_USER;
    SDL_UserEvent& userEvent = event.user;
    userEvent.type = SDL_EVENT_USER;
    userEvent.code = 1;
    rad::Application::GetInstance()->PushEvent(event);

    return interval;
}

bool WindowTest::Init()
{
    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
    Create("WindowTest", 800, 600, flags);
    m_renderer = new rad::Renderer(this);
    m_renderer->Init();
    m_powerWatchdog = new rad::Timer();
    m_powerWatchdog->StartMS(1000, PowerWatchdog, this);

    uint32_t index = 0;
    for (auto driver : rad::GetAudioDrivers())
    {
        RAD_LOG(m_logger, info, "Audio Driver#{}: {}", index++, driver);
    }
    const char* audioDriver = rad::GetCurrentAudioDriver();
    RAD_LOG(m_logger, info, "Current Audio Driver: {}", audioDriver ? audioDriver : "NA");

    if (rad::LoadWAVFromFile("sample.wav", &m_wave.spec, &m_wave.sound, &m_wave.soundSizeInBytes))
    {
        m_audioStream = rad::AudioStream::CreateDefaultPlayback(&m_wave.spec);
        m_audioStream->Resume();
    }
    return true;
}

bool WindowTest::OnEvent(const SDL_Event& event)
{
    return rad::Window::OnEvent(event);
}

void WindowTest::OnIdle()
{
    if (m_renderer)
    {
        m_renderer->Clear();
        m_renderer->Present();
    }

    if (m_audioStream)
    {
        const int minQueuedBytes = (m_wave.soundSizeInBytes / SDL_AUDIO_FRAMESIZE(m_wave.spec)) / 2;
        if (m_audioStream->GetDataSizeQueued() < minQueuedBytes)
        {
            m_audioStream->PutData(m_wave.sound, m_wave.soundSizeInBytes);
        }
    }
}

void WindowTest::OnShown()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnHidden()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnExposed()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnMoved(int x, int y)
{
    RAD_LOG(m_logger, trace, "OnMoved: x={:4}; y={:4};", x, y);
}

void WindowTest::OnResized(int width, int height)
{
    RAD_LOG(m_logger, trace, "OnResized: width={:4}; height={:4};", width, height);
}

void WindowTest::OnPixelSizeChanged(int width, int height)
{
    RAD_LOG(m_logger, trace, "OnPixelSizeChanged: width={:4}; height={:4};", width, height);
}

void WindowTest::OnMinimized()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnMaximized()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnRestored()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnMouseEnter()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnMouseLeave()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnFocusGained()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnFocusLost()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnCloseRequested()
{
    RAD_LOG(m_logger, trace, __func__);
    Destroy();
}

void WindowTest::OnTakeFocus()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnHitTest()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnIccProfileChanged()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnDisplayChanged()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnDisplayScaleChanged()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnOccluded()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnEnterFullscreen()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnLeaveFullscreen()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnDestroyed()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnPenEnter()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnPenLeave()
{
    RAD_LOG(m_logger, trace, __func__);
}

void WindowTest::OnKeyDown(const SDL_KeyboardEvent& keyDown)
{
    RAD_LOG(m_logger, trace, "OnKeyDown: {}", SDL_GetKeyName(keyDown.key));
}

void WindowTest::OnKeyUp(const SDL_KeyboardEvent& keyUp)
{
    RAD_LOG(m_logger, trace, "OnKeyUp: {}", SDL_GetKeyName(keyUp.key));
}

void WindowTest::OnTextEditing(const SDL_TextEditingEvent& textEditing)
{
    RAD_LOG(m_logger, trace, "OnTextEditing: {}", textEditing.text);
}

void WindowTest::OnTextInput(const SDL_TextInputEvent& textInput)
{
    RAD_LOG(m_logger, trace, "OnTextInput: {}", textInput.text);
}

void WindowTest::OnMouseMove(const SDL_MouseMotionEvent& mouseMotion)
{
    RAD_LOG(m_logger, trace, "OnMouseMove: x={:4} ({:+4}); y={:4} ({:+4})",
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
    RAD_LOG(m_logger, trace, "OnMouseButtonDown: {}", GetMouseButtonName(mouseButton.button));
}

void WindowTest::OnMouseButtonUp(const SDL_MouseButtonEvent& mouseButton)
{
    RAD_LOG(m_logger, trace, "OnMouseButtonUp: {}", GetMouseButtonName(mouseButton.button));
}

void WindowTest::OnMouseWheel(const SDL_MouseWheelEvent& mouseWheel)
{
    RAD_LOG(m_logger, trace, "OnMouseWheel: x={:+2}; y={:+2}",
        mouseWheel.x, mouseWheel.y);
}

void WindowTest::OnUserEvent(const SDL_UserEvent& user)
{
    RAD_LOG(m_logger, info, "OnUserEvent({})", user.code);
}
