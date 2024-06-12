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
        RAD_LOG(Gui, info, "Working directory: {}", SDL_GetBasePath());
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

} // namespace rad
