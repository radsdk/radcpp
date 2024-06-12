#pragma once

#include "rad/Core/Platform.h"
#include "rad/Core/Integer.h"
#include "rad/Core/String.h"
#include "rad/IO/Logging.h"
#include "SDL3/SDL.h"

namespace rad
{

spdlog::logger* GetGuiLogger();

class Application
{
public:
    Application();
    ~Application();

    bool Init(int argc, char** argv);
    void Destroy();

    SDL_InitFlags GetInitialized();
    bool IsInitialized(SDL_InitFlags flags);

private:

}; // class Application

} // namespace rad
