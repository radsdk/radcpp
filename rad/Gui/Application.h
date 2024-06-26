#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/Integer.h>
#include <rad/Core/String.h>
#include <rad/Core/RefCounted.h>
#include <rad/IO/Logging.h>
#include <rad/Gui/EventHandler.h>
#include <SDL3/SDL.h>
#include <atomic>
#include <mutex>

namespace rad
{

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

class Application : public RefCounted<Application>
{
public:
    Application();
    ~Application();

    static Application* GetInstance();

    bool Init(int argc, char** argv);
    void Destroy();

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
    // Caller must call "SDL_free" on the returned pointer when done with it.
    void* GetClipboardData(const char* mimeType, size_t* size);
    bool HasClipboardData(const char* mimeType);

private:
    void UpdateDisplayInfos();
    static Application* s_singleton;
    std::vector<DisplayInfo> m_displays;
    std::mutex m_eventMutex;
    std::vector<EventHandler*> m_eventHandlers;

    std::atomic_bool m_exit = false;

}; // class Application

} // namespace rad
