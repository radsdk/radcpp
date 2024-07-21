#include <rad/Gui/Application.h>

namespace rad
{

spdlog::logger* GetGuiLogger()
{
    static std::shared_ptr<spdlog::logger> logger = CreateLogger("Gui");
    return logger.get();
}

static Application* g_app = nullptr;

Application* GetApp()
{
    return g_app;
}

Application::Application()
{
    assert(g_app == nullptr);
    g_app = this;
}

Application::~Application()
{
    Destroy();
}

bool Application::Init(int argc, char** argv)
{
    Program::Init(argc, argv);
    m_argc = argc;
    m_argv = argv;
    int result = SDL_Init(
        SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO |
        SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD);
    if (result == 0)
    {
        int version = SDL_GetVersion();
        auto logger = GetGuiLogger();
        RAD_LOG(logger, info, "SDL initialized on {}: {}.{}.{} ({})", SDL_GetPlatform(),
            SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version),
            SDL_GetRevision());
        if (const char* pWorkingDir = SDL_GetBasePath())
        {
            RAD_LOG(logger, info, "Working Directory: {}", pWorkingDir);
        }
        if (const char* pVideoDriver = SDL_GetCurrentVideoDriver())
        {
            RAD_LOG(logger, info, "Current Video Driver: {}", pVideoDriver);
        }
        if (const char* pAudioDriver = SDL_GetCurrentAudioDriver())
        {
            RAD_LOG(logger, info, "Current Audio Driver: {}", pAudioDriver);
        }
        UpdateDisplayInfos();
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_Init failed: {}", SDL_GetError());
        return false;
    }
}

void Application::Destroy()
{
    if (g_app)
    {
        SDL_Quit();
        RAD_LOG(GetGuiLogger(), info, "SDL quited.");
        g_app = nullptr;
    }
}

const char* Application::GetError()
{
    return SDL_GetError();
}

SDL_InitFlags Application::GetInitialized()
{
    return SDL_WasInit(0);
}

bool Application::IsSubsystemInitialized(SDL_InitFlags flags)
{
    SDL_InitFlags mask = SDL_WasInit(flags);
    return HasBits(flags, mask);
}

void Application::RegisterEventHandler(EventHandler* handler)
{
    std::lock_guard<std::mutex> lock(m_eventMutex);
    m_eventHandlers.push_back(handler);
}

void Application::UnregisterEventHandler(EventHandler* handler)
{
    std::lock_guard<std::mutex> lock(m_eventMutex);
    std::erase(m_eventHandlers, handler);
}

bool Application::PushEvent(SDL_Event& event)
{
    int result = SDL_PushEvent(&event);
    if (result == 1)
    {
        return true;
    }
    else
    {
        if (result < 0)
        {
            RAD_LOG(GetGuiLogger(), err, "SDL_PushEvent failed: {}", SDL_GetError());
        }
        return false;
    }
}

void Application::OnEvent(const SDL_Event& event)
{
    for (EventHandler* handler : m_eventHandlers)
    {
        if (handler->OnEvent(event))
        {
            return;
        }
    }

    auto logger = GetGuiLogger();
    switch (event.type)
    {
    case SDL_EVENT_QUIT: // User-requested quit.
        break;
    case SDL_EVENT_TERMINATING:
        RAD_LOG(logger, info, "SDL_EVENT_TERMINATING: "
            "The application is being terminated by the OS.");
        break;
    case SDL_EVENT_LOW_MEMORY:
        RAD_LOG(logger, info, "SDL_EVENT_LOW_MEMORY: "
            "The application is low on memory, free memory if possible.");
        break;
    case SDL_EVENT_WILL_ENTER_BACKGROUND:
        RAD_LOG(logger, info, "SDL_EVENT_WILL_ENTER_BACKGROUND: "
            "The application is about to enter the background.");
        break;
    case SDL_EVENT_DID_ENTER_BACKGROUND:
        RAD_LOG(logger, info, "SDL_EVENT_DID_ENTER_BACKGROUND: "
            "The application did enter the background and may not get CPU for some time.");
        break;
    case SDL_EVENT_WILL_ENTER_FOREGROUND:
        RAD_LOG(logger, info, "SDL_EVENT_WILL_ENTER_FOREGROUND: "
            "The application is about to enter the foreground.");
        break;
    case SDL_EVENT_DID_ENTER_FOREGROUND:
        RAD_LOG(logger, info, "SDL_EVENT_DID_ENTER_FOREGROUND: "
            "The application is now interactive.");
        break;
    case SDL_EVENT_LOCALE_CHANGED:
        RAD_LOG(logger, info, "SDL_EVENT_LOCALE_CHANGED: "
            "The user's locale preferences have changed.");
        break;
    case SDL_EVENT_SYSTEM_THEME_CHANGED:
        RAD_LOG(logger, info, "SDL_EVENT_SYSTEM_THEME_CHANGED: "
            "The system theme changed.");
        break;
    case SDL_EVENT_DISPLAY_ORIENTATION:
    case SDL_EVENT_DISPLAY_ADDED:
    case SDL_EVENT_DISPLAY_REMOVED:
    case SDL_EVENT_DISPLAY_MOVED:
    case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
        RAD_LOG(logger, info, "Display state changed.");
        UpdateDisplayInfos();
        break;
    }
}

void Application::OnIdle()
{
    for (EventHandler* handler : m_eventHandlers)
    {
        handler->OnIdle();
    }

    if (m_eventHandlers.empty())
    {
        m_exit = true;
    }
}

bool Application::IsScreenSaverEnabled()
{
    return (SDL_ScreenSaverEnabled() == SDL_TRUE);
}

bool Application::EnableScreenSaver()
{
    int result = SDL_EnableScreenSaver();
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_EnableScreenSaver failed: {}", SDL_GetError());
        return false;
    }
}

bool Application::DisableScreenSaver()
{
    int result = SDL_DisableScreenSaver();
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_DisableScreenSaver failed: {}", SDL_GetError());
        return false;
    }
}

bool Application::SetClipboardText(const char* text)
{
    int result = SDL_SetClipboardText(text);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetClipboardText failed: {}", SDL_GetError());
        return false;
    }
}

std::string Application::GetClipboardText()
{
    std::string buffer;
    const char* text = SDL_GetClipboardText();
    if (text)
    {
        buffer = text;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetClipboardText failed: {}", SDL_GetError());
    }
    return buffer;
}

bool Application::HasClipboardText()
{
    return (SDL_HasClipboardText() == SDL_TRUE);
}

bool Application::SetPrimarySelectionText(const char* text)
{
    int result = SDL_SetPrimarySelectionText(text);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetPrimarySelectionText failed: {}", SDL_GetError());
        return false;
    }
}

std::string Application::GetPrimarySelectionText()
{
    std::string buffer;
    const char* text = SDL_GetPrimarySelectionText();
    if (text)
    {
        buffer = text;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetPrimarySelectionText failed: {}", SDL_GetError());
    }
    return buffer;
}

bool Application::HasPrimarySelectionText()
{
    return (SDL_HasPrimarySelectionText() == SDL_TRUE);
}

bool Application::SetClipboardData(SDL_ClipboardDataCallback callback, SDL_ClipboardCleanupCallback cleanup,
    void* userData, const char** mimeTypes, size_t mimeTypeCount)
{
    int result = SDL_SetClipboardData(callback, cleanup, userData, mimeTypes, mimeTypeCount);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetClipboardData failed: {}", SDL_GetError());
        return false;
    }
}

bool Application::ClearClipboardData()
{
    return (SDL_ClearClipboardData() == SDL_TRUE);
}

const void* Application::GetClipboardData(const char* mimeType, size_t* size)
{
    if (const void* data = SDL_GetClipboardData(mimeType, size))
    {
        return data;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetClipboardData failed: {}", SDL_GetError());
        return nullptr;
    }
}

bool Application::HasClipboardData(const char* mimeType)
{
    return (SDL_HasClipboardData(mimeType) == SDL_TRUE);
}

void Application::UpdateDisplayInfos()
{
    int displayCount = 0;
    const SDL_DisplayID* ids = SDL_GetDisplays(&displayCount);
    if (ids == nullptr)
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetDisplays failed: {}", SDL_GetError());
        m_displays.clear();
        return;
    }
    m_displays.clear();
    m_displays.resize(displayCount);
    for (int i = 0; i < displayCount; ++i)
    {
        DisplayInfo info = {};
        SDL_DisplayID id = ids[i];
        info.id = id;
        if (const char* pName = SDL_GetDisplayName(id))
        {
            info.name = pName;
        }
        else
        {
            RAD_LOG(GetGuiLogger(), err, "SDL_GetDisplayName failed: {}", SDL_GetError());
            info.name = "Unknown";
        }

        if (SDL_GetDisplayBounds(id, &info.bounds) < 0)
        {
            RAD_LOG(GetGuiLogger(), err, "SDL_GetDisplayBounds failed: {}", SDL_GetError());
        }

        if (SDL_GetDisplayUsableBounds(id, &info.usableBounds) < 0)
        {
            RAD_LOG(GetGuiLogger(), err, "SDL_GetDisplayUsableBounds failed: {}", SDL_GetError());
        }

        info.naturalOrientation = SDL_GetNaturalDisplayOrientation(id);
        info.currentOrientation = SDL_GetCurrentDisplayOrientation(id);

        info.scale = SDL_GetDisplayContentScale(id);
        if (info.scale == 0.0f)
        {
            RAD_LOG(GetGuiLogger(), err, "SDL_GetDisplayContentScale failed: {}", SDL_GetError());
        }

        info.propID = SDL_GetDisplayProperties(id);
        if (info.propID != 0)
        {
            info.hdrEnabled = SDL_GetBooleanProperty(info.propID,
                SDL_PROP_DISPLAY_HDR_ENABLED_BOOLEAN, SDL_FALSE);
            info.kmsdrmOrientation = SDL_GetNumberProperty(info.propID,
                SDL_PROP_DISPLAY_KMSDRM_PANEL_ORIENTATION_NUMBER, 0);
        }
        else
        {
            RAD_LOG(GetGuiLogger(), err, "SDL_GetDisplayProperties failed: {}", SDL_GetError());
        }

        int count = 0;
        const SDL_DisplayMode* const* modes = SDL_GetFullscreenDisplayModes(id, &count);
        if (modes && (count > 0))
        {
            info.modes.resize(count);
            std::memcpy(info.modes.data(), modes, count * sizeof(modes[0]));
        }

        info.desktopMode = SDL_GetDesktopDisplayMode(id);
        info.currentMode = SDL_GetCurrentDisplayMode(id);

        RAD_LOG(GetGuiLogger(), info, "Display#{}: {} ({}x{}@{}Hz, {})",
            i, info.name, info.currentMode->w, info.currentMode->h,
            info.currentMode->refresh_rate,
            SDL_GetPixelFormatName(info.currentMode->format));
    }
}

} // namespace rad
