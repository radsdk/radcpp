#include "Application.h"

namespace rad
{

spdlog::logger* GetGuiLogger()
{
    static std::shared_ptr<spdlog::logger> guiLogger = CreateLogger("Gui");
    return guiLogger.get();
}

Application::Application()
{
}

Application::~Application()
{
}

bool Application::Init(int argc, char** argv)
{
    int res = SDL_Init(
        SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO |
        SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD);
    if (res == 0)
    {
        int version = SDL_GetVersion();
        RAD_LOG(Gui, info, "SDL initialized on {}: {}.{}.{} ({})", SDL_GetPlatform(),
            SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version),
            SDL_GetRevision());
        RAD_LOG(Gui, info, "Working Directory: {}", SDL_GetBasePath());
        RAD_LOG(Gui, info, "Video Driver: {}", SDL_GetCurrentVideoDriver());
        RAD_LOG(Gui, info, "Audio Driver: {}", SDL_GetCurrentAudioDriver());
        UpdateDisplayInfos();
        return true;
    }
    else
    {
        RAD_LOG(Gui, err, "SDL_Init failed: {}", SDL_GetError());
        return false;
    }
}

void Application::Destroy()
{
    SDL_Quit();
    RAD_LOG(Gui, info, "SDL quited.");
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

void Application::UpdateDisplayInfos()
{
    int displayCount = 0;
    SDL_DisplayID* ids = SDL_GetDisplays(&displayCount);
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
            RAD_LOG(Gui, err, "SDL_GetDisplayBounds failed: {}", SDL_GetError());
        }
        if (SDL_GetDisplayUsableBounds(id, &info.usableBounds) != 0)
        {
            RAD_LOG(Gui, err, "SDL_GetDisplayUsableBounds failed: {}", SDL_GetError());
        }

        info.naturalOrientation = SDL_GetNaturalDisplayOrientation(id);
        info.currentOrientation = SDL_GetCurrentDisplayOrientation(id);

        info.scale = SDL_GetDisplayContentScale(id);
        if (info.scale == 0.0f)
        {
            RAD_LOG(Gui, err, "SDL_GetDisplayContentScale failed: {}", SDL_GetError());
        }

        info.propID = SDL_GetDisplayProperties(id);
        if (info.propID == 0)
        {
            info.hdrEnabled = SDL_GetBooleanProperty(info.propID, SDL_PROP_DISPLAY_HDR_ENABLED_BOOLEAN, SDL_FALSE);
            info.sdrWhitePoint = SDL_GetFloatProperty(info.propID, SDL_PROP_DISPLAY_SDR_WHITE_POINT_FLOAT, 1.0f);
            info.hdrHeadroom = SDL_GetFloatProperty(info.propID, SDL_PROP_DISPLAY_HDR_HEADROOM_FLOAT, 1.0f);
            info.kmsdrmOrientation = SDL_GetNumberProperty(info.propID, SDL_PROP_DISPLAY_KMSDRM_PANEL_ORIENTATION_NUMBER, 0);
        }
        else
        {
            RAD_LOG(Gui, err, "SDL_GetDisplayProperties failed: {}", SDL_GetError());
        }

        int count = 0;
        const SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(id, &count);
        if (count > 0)
        {
            info.modes.resize(count);
            std::memcpy(info.modes.data(), modes, count * sizeof(modes[0]));
        }
        info.desktopMode = SDL_GetDesktopDisplayMode(id);
        info.currentMode = SDL_GetCurrentDisplayMode(id);
    }
}

} // namespace rad
