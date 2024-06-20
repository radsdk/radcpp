#pragma once

#include <rad/Core/Platform.h>
#include <SDL3/SDL_power.h>

namespace rad
{

const char* GetPowerStateString(SDL_PowerState state);
SDL_PowerState GetPowerInfo(int* seconds, int* percent);

} // namespace rad
