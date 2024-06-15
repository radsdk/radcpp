#include <rad/Gui/Application.h>

namespace rad
{

spdlog::logger* GetGuiLogger()
{
    static std::shared_ptr<spdlog::logger> logger = CreateLogger("Gui");
    return logger.get();
}

Application* Application::s_singleton = nullptr;

Application::Application()
{
    assert(s_singleton == nullptr);
    s_singleton = this;
}

Application::~Application()
{
    s_singleton = nullptr;
}

Application* Application::GetInstance()
{
    return s_singleton;
}

bool Application::Init(int argc, char** argv)
{
    auto logger = GetGuiLogger();
    int res = SDL_Init(
        SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO |
        SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD);
    if (res == 0)
    {
        int version = SDL_GetVersion();
        RAD_LOG(logger, info, "SDL initialized on {}: {}.{}.{} ({})", SDL_GetPlatform(),
            SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version),
            SDL_GetRevision());
        RAD_LOG(logger, info, "Working Directory: {}", SDL_GetBasePath());
        RAD_LOG(logger, info, "Video Driver: {}", SDL_GetCurrentVideoDriver());
        RAD_LOG(logger, info, "Audio Driver: {}", SDL_GetCurrentAudioDriver());
        UpdateDisplayInfos();
        return true;
    }
    else
    {
        RAD_LOG(logger, err, "SDL_Init failed: {}", SDL_GetError());
        return false;
    }
}

void Application::Destroy()
{
    auto logger = GetGuiLogger();
    SDL_Quit();
    RAD_LOG(logger, info, "SDL quited.");
}

SDL_InitFlags Application::GetInitialized()
{
    return SDL_WasInit(0);
}

bool Application::IsInitialized(SDL_InitFlags flags)
{
    SDL_InitFlags res = SDL_WasInit(flags);
    return HasBits(flags, res);
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
    case SDL_EVENT_DISPLAY_HDR_STATE_CHANGED:
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

void Application::UpdateDisplayInfos()
{
    auto logger = GetGuiLogger();

    int displayCount = 0;
    SDL_DisplayID* ids = SDL_GetDisplays(&displayCount);
    if (ids == nullptr)
    {
        RAD_LOG(logger, err, "SDL_GetDisplays failed: {}", SDL_GetError());
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
        info.name = SDL_GetDisplayName(id);

        if (SDL_GetDisplayBounds(id, &info.bounds) != 0)
        {
            RAD_LOG(logger, err, "SDL_GetDisplayBounds failed: {}", SDL_GetError());
        }
        if (SDL_GetDisplayUsableBounds(id, &info.usableBounds) != 0)
        {
            RAD_LOG(logger, err, "SDL_GetDisplayUsableBounds failed: {}", SDL_GetError());
        }

        info.naturalOrientation = SDL_GetNaturalDisplayOrientation(id);
        info.currentOrientation = SDL_GetCurrentDisplayOrientation(id);

        info.scale = SDL_GetDisplayContentScale(id);
        if (info.scale == 0.0f)
        {
            RAD_LOG(logger, err, "SDL_GetDisplayContentScale failed: {}", SDL_GetError());
        }

        info.propID = SDL_GetDisplayProperties(id);
        if (info.propID != 0)
        {
            info.hdrEnabled = SDL_GetBooleanProperty(info.propID, SDL_PROP_DISPLAY_HDR_ENABLED_BOOLEAN, SDL_FALSE);
            info.sdrWhitePoint = SDL_GetFloatProperty(info.propID, SDL_PROP_DISPLAY_SDR_WHITE_POINT_FLOAT, 1.0f);
            info.hdrHeadroom = SDL_GetFloatProperty(info.propID, SDL_PROP_DISPLAY_HDR_HEADROOM_FLOAT, 1.0f);
            info.kmsdrmOrientation = SDL_GetNumberProperty(info.propID, SDL_PROP_DISPLAY_KMSDRM_PANEL_ORIENTATION_NUMBER, 0);
        }
        else
        {
            RAD_LOG(logger, err, "SDL_GetDisplayProperties failed: {}", SDL_GetError());
        }

        int count = 0;
        const SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(id, &count);
        if ((modes != nullptr) && (count > 0))
        {
            info.modes.resize(count);
            std::memcpy(info.modes.data(), modes, count * sizeof(modes[0]));
            SDL_free(modes);
        }
        info.desktopMode = SDL_GetDesktopDisplayMode(id);
        info.currentMode = SDL_GetCurrentDisplayMode(id);

        RAD_LOG(logger, info, "Display#{}: {} ({}x{}@{}, {})",
            i, info.name, info.currentMode->w, info.currentMode->h,
            info.currentMode->refresh_rate,
            SDL_GetPixelFormatName(info.currentMode->format));
    }
}

} // namespace rad
