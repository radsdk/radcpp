#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/String.h"
#include "EventHandler.h"
#include "SDL2/SDL.h"

#include <atomic>
#include <set>

namespace sdl
{

struct DisplayInfo
{
    std::string name;
    SDL_Rect bounds;
    SDL_Rect usableBounds;
    float ddpi;
    float hdpi;
    float vdpi;
    std::vector<SDL_DisplayMode> modes;
}; // struct DisplayInfo

class Application : public rad::RefCounted<Application>
{
public:
    static Application* GetInstance() { return s_instance; }

    Application();
    virtual ~Application();

    bool Init(int argc, char** argv);
    void Destroy();

    const char* GetError();

    bool LoadVulkanLibrary(const char* path = nullptr);
    std::set<std::string> GetVulkanInstanceExtensions();

    void RegisterEventHandler(EventHandler* handler);
    void UnregisterEventHandler(EventHandler* handler);

    int Run();

    void SetExitEventLoop(bool exit = true) { m_exitEventLoop = exit; }
    void SetExitCode(int exitCode) { m_exitCode = exitCode; }

    const std::vector<std::string>& GetCommandLineArgs() const { return m_commandLineArgs; }
    const std::vector<DisplayInfo>& GetDisplayInfos() const { return m_displayInfos; }

    static std::string GetEnvironment(std::string_view name);

    // Returns the average of horizontal and vertical DPI; Return default 96.0f if failed.
    float GetDisplayDPI(int displayIndex);

protected:
    bool OnEvent(const SDL_Event& event);
    void UpdateDisplayInfos();

    static Application* s_instance;

    // Handlers registered for the event loop
    std::vector<EventHandler*> m_eventHandlers;
    std::atomic_bool m_exitEventLoop = false;
    int m_exitCode = 0;

    std::vector<std::string> m_commandLineArgs;
    std::vector<DisplayInfo> m_displayInfos;

    bool m_isVulkanLibraryLoaded = false;

}; // class Application

} // namespace sdl
