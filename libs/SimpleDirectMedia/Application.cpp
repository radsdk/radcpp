#include "Application.h"
#include "Window.h"
#include "rad/IO/Logging.h"

#include "SDL2/SDL_vulkan.h"

#if defined(_WIN32)
#include <Windows.h>
#endif

using namespace rad;

namespace sdl
{

Application* Application::s_instance = nullptr;

Application::Application()
{
    assert(s_instance == nullptr);
    s_instance = this;
}

Application::~Application()
{
    Destroy();
}

bool Application::Init(int argc, char** argv)
{
#if defined(_WIN32) || defined(_WIN64)
    ::SetConsoleOutputCP(65001);
#endif

    std::string commandLine;
    for (int i = 0; i < argc; ++i)
    {
        m_commandLineArgs.push_back(argv[i]);
        commandLine += std::string(argv[i]) + " ";
    }
    commandLine.pop_back();
    LogGlobal(Info, "Command line: %s", commandLine.c_str());

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        LogGlobal(Error, "SDL_Init failed: %s", SDL_GetError());
    }

    SDL_version linkedVersion = {};
    SDL_GetVersion(&linkedVersion);
    LogGlobal(Info, "SDL initialized: Version: %d.%d.%d",
        linkedVersion.major, linkedVersion.minor, linkedVersion.patch);

    LogGlobal(Info, "Platform: %s", SDL_GetPlatform());
    char* basePath = SDL_GetBasePath();
    LogGlobal(Info, "BasePath: %s", basePath);
    SDL_free(basePath);

    UpdateDisplayInfos();

    return true;
}

void Application::Destroy()
{
    if (m_isVulkanLibraryLoaded)
    {
        SDL_Vulkan_UnloadLibrary();
    }

    SDL_Quit();
    LogGlobal(Info, "SDL Quited.");
}

const char* Application::GetError()
{
    return SDL_GetError();
}

bool Application::LoadVulkanLibrary(const char* path)
{
    if (SDL_Vulkan_LoadLibrary(path) == 0)
    {
        m_isVulkanLibraryLoaded = true;
        return true;
    }
    else
    {
        LogGlobal(Error, "SDL_Vulkan_LoadLibrary(path=%s): %s",
            path ? path : "nullptr", SDL_GetError());
        return false;
    }
}

std::set<std::string> Application::GetVulkanInstanceExtensions()
{
    std::vector<const char*> extensionNames;
    unsigned int extensionCount = 0;
    if (!SDL_Vulkan_GetInstanceExtensions(NULL, &extensionCount, NULL))
    {
        LogGlobal(Error, "SDL_Vulkan_GetInstanceExtensions failed");
    }
    extensionNames.resize(extensionCount);
    if (!SDL_Vulkan_GetInstanceExtensions(NULL, &extensionCount, extensionNames.data()))
    {
        LogGlobal(Error, "SDL_Vulkan_GetInstanceExtensions failed");
    }
    return { extensionNames.begin(), extensionNames.end() };
}

void Application::RegisterEventHandler(EventHandler* handler)
{
    m_eventHandlers.push_back(handler);
}

void Application::UnregisterEventHandler(EventHandler* handler)
{
    std::erase(m_eventHandlers, handler);
}

int Application::Run()
{
    while (!m_exitEventLoop)
    {
        SDL_Event event = {};
        while (SDL_PollEvent(&event))
        {
            OnEvent(event);
        }

        for (EventHandler* handler : m_eventHandlers)
        {
            handler->OnIdle();
        }

        if (m_eventHandlers.empty())
        {
            m_exitEventLoop = true;
        }
    }
    return m_exitCode;
}

bool Application::OnEvent(const SDL_Event& event)
{
    for (EventHandler* handler : m_eventHandlers)
    {
        if (handler->OnEvent(event))
        {
            return true;
        }
    }

    switch (event.type)
    {
    case SDL_QUIT:
        m_exitEventLoop = true;
        break;
    case SDL_APP_TERMINATING:
        LogGlobal(Info, "SDL_APP_TERMINATING: The application is being terminated by the OS.");
        break;
    case SDL_APP_LOWMEMORY:
        LogGlobal(Info, "SDL_APP_LOWMEMORY: The application is low on memory, free memory if possible.");
        break;
    case SDL_APP_WILLENTERBACKGROUND:
        LogGlobal(Info, "SDL_APP_WILLENTERBACKGROUND: The application is about to enter the background.");
        break;
    case SDL_APP_DIDENTERBACKGROUND:
        LogGlobal(Info, "SDL_APP_DIDENTERBACKGROUND: The application did enter the background and may not get CPU for some time.");
        break;
    case SDL_APP_WILLENTERFOREGROUND:
        LogGlobal(Info, "SDL_APP_WILLENTERFOREGROUND: The application is about to enter the foreground.");
        break;
    case SDL_APP_DIDENTERFOREGROUND:
        LogGlobal(Info, "SDL_APP_DIDENTERFOREGROUND: The application is now interactive.");
        break;
    case SDL_LOCALECHANGED:
        LogGlobal(Info, "SDL_LOCALECHANGED: The user's locale preferences have changed.");
        break;
    case SDL_DISPLAYEVENT:
        LogGlobal(Info, "SDL_DISPLAYEVENT: Display state change.");
        UpdateDisplayInfos();
        break;
    }

    return true;
}

void Application::UpdateDisplayInfos()
{
    m_displayInfos.resize(SDL_GetNumVideoDisplays());
    for (int displayIndex = 0; displayIndex < static_cast<int>(m_displayInfos.size()); displayIndex++)
    {
        DisplayInfo& displayInfo = m_displayInfos[displayIndex];
        displayInfo.name = SDL_GetDisplayName(displayIndex);
        SDL_GetDisplayBounds(displayIndex, &displayInfo.bounds);
        SDL_GetDisplayUsableBounds(displayIndex, &displayInfo.usableBounds);
        SDL_GetDisplayDPI(displayIndex, &displayInfo.ddpi, &displayInfo.hdpi, &displayInfo.vdpi);
        displayInfo.modes.resize(SDL_GetNumDisplayModes(displayIndex));
        for (int modeIndex = 0; modeIndex < displayInfo.modes.size(); modeIndex++)
        {
            SDL_GetDisplayMode(displayIndex, modeIndex, &displayInfo.modes[modeIndex]);
        }
    }
}

std::string Application::GetEnvironment(std::string_view name)
{
    std::string value;
#ifdef _WIN32
    std::wstring nameWide = StrU8ToWide(name);
    wchar_t* pBuffer = nullptr;
    size_t bufferSize = 0;
    errno_t err = _wdupenv_s(&pBuffer, &bufferSize, nameWide.data());
    if (err == 0)
    {
        if (pBuffer)
        {
            value = StrWideToU8(pBuffer);
            free(pBuffer);
        }
    }
#else
    value = std::getenv(name.data());
#endif
    return value;
}

float Application::GetDisplayDPI(int displayIndex)
{
    float ddpi = 96.0f;
    float hdpi = 96.0f;
    float vdpi = 96.0f;
    if (SDL_GetDisplayDPI(displayIndex, &ddpi, &hdpi, &vdpi) != 0)
    {
        LogGlobal(Error, "SDL_GetDisplayDPI: %s", SDL_GetError());
    }
    return (hdpi + vdpi) / 2.0f;
}

} // namespace sdl
