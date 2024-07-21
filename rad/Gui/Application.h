#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/Integer.h>
#include <rad/Core/Memory.h>
#include <rad/System/Program.h>
#include <rad/IO/Logging.h>
#include <rad/Gui/EventHandler.h>
#include <SDL3/SDL.h>
#include <atomic>
#include <mutex>

namespace rad
{

spdlog::logger* GetGuiLogger();

struct DisplayInfo
{
    SDL_DisplayID id;
    const char* name;
    SDL_Rect bounds;
    // This is the same area as bounds, but with portions reserved by the system removed.
    SDL_Rect usableBounds;
    SDL_DisplayOrientation naturalOrientation;
    SDL_DisplayOrientation currentOrientation;
    // The content scale is the expected scale for content based on the DPI settings of the display.
    float scale;

    // https://wiki.libsdl.org/SDL3/SDL_GetDisplayProperties
    SDL_PropertiesID propID;
    SDL_bool hdrEnabled;
    float sdrWhitePoint;
    float hdrHeadroom;
    Sint64 kmsdrmOrientation;

    std::vector<const SDL_DisplayMode*> modes;
    const SDL_DisplayMode* desktopMode;
    const SDL_DisplayMode* currentMode;

}; // struct DisplayInfo


class Application : public Program
{
public:
    Application();
    ~Application();

    bool Init(int argc, char** argv);
    void Destroy();

    int GetArgc() const { return m_argc; }
    char** GetArgv() const { return m_argv; }

    const char* GetError();

    SDL_InitFlags GetInitialized();
    bool IsSubsystemInitialized(SDL_InitFlags flags);

    const std::vector<DisplayInfo>& GetDisplayInfos() { return m_displays; }

    void RegisterEventHandler(EventHandler* handler);
    void UnregisterEventHandler(EventHandler* handler);
    // Return true on success; false if the event is filtered or on failure (event queue being full).
    bool PushEvent(SDL_Event& event);
    void OnEvent(const SDL_Event& event);
    void OnIdle();

    void SetExit(bool exit) { m_exit = exit; }
    bool GetExit() { return m_exit; }

    bool IsScreenSaverEnabled();
    bool EnableScreenSaver();
    bool DisableScreenSaver();

    // Put UTF-8 text into the clipboard.
    bool SetClipboardText(const char* text);
    std::string GetClipboardText();
    bool HasClipboardText();

    // Put UTF-8 text into the primary selection.
    bool SetPrimarySelectionText(const char* text);
    std::string GetPrimarySelectionText();
    bool HasPrimarySelectionText();

    // Tell the operating system that the application is offering clipboard data
    // for each of the proivded mime types.
    bool SetClipboardData(SDL_ClipboardDataCallback callback, SDL_ClipboardCleanupCallback cleanup,
        void* userData, const char** mimeTypes, size_t mimeTypeCount);
    bool ClearClipboardData();
    const void* GetClipboardData(const char* mimeType, size_t* size);
    bool HasClipboardData(const char* mimeType);

private:
    void UpdateDisplayInfos();
    int m_argc = 0;
    char** m_argv = nullptr;
    std::vector<DisplayInfo> m_displays;
    std::mutex m_eventMutex;
    std::vector<EventHandler*> m_eventHandlers;

    std::atomic_bool m_exit = false;

}; // class Application

Application* GetApp();

// Get the current value of the high resolution counter.
Uint64 GetPerfCounter();
// Get the frequency in Hz of the high resolution counter.
Uint64 GetPerfCounterFrequency();

// Wait a specified number of milliseconds before returning.
void DelayMS(Uint32 ms);
// Wait a specified number of nanoseconds before returning.
void DelayNS(Uint64 ns);

namespace sdl
{

using TimerCallbackMS = SDL_TimerCallback;
using TimerCallbackNS = SDL_NSTimerCallback;

class Timer : public rad::RefCounted<Timer>
{
public:
    Timer();
    virtual ~Timer();

    bool StartMS(Uint32 interval, TimerCallbackMS callback, void* userData);
    bool StartNS(Uint64 interval, TimerCallbackNS callback, void* userData);
    void Stop();

    SDL_TimerID GetID() const { return m_id; }

protected:
    SDL_TimerID m_id = 0;
}; // class Timer

class TimerTaskMS : public Timer
{
public:
    // SDL_TimerCallback
    virtual Uint32 Callback(SDL_TimerID timerID, Uint32 interval) = 0;
    bool Start(Uint32 interval);

}; // class TimerTaskMS

class TimerTaskNS : public Timer
{
public:
    // SDL_NSTimerCallback
    virtual Uint64 Callback(SDL_TimerID timerID, Uint64 interval) = 0;
    bool Start(Uint64 interval);

}; // class TimerTaskMS

} // namespace sdl

} // namespace rad
