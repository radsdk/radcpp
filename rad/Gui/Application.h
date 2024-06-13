#pragma once

#include "rad/Core/Platform.h"
#include "rad/Core/Integer.h"
#include "rad/Core/String.h"
#include "rad/IO/Logging.h"
#include "SDL3/SDL.h"

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

class Application
{
public:
    Application();
    ~Application();

    bool Init(int argc, char** argv);
    void Destroy();

    SDL_InitFlags GetInitialized();
    bool IsInitialized(SDL_InitFlags flags);

    const std::vector<DisplayInfo>& GetDisplayInfos() { return m_displays; }

private:
    void UpdateDisplayInfos();
    std::vector<DisplayInfo> m_displays;

}; // class Application

} // namespace rad
