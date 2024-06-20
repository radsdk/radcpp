#include <rad/System/Power.h>

namespace rad
{

const char* GetPowerStateString(SDL_PowerState state)
{
    switch (state)
    {
    case SDL_POWERSTATE_ON_BATTERY: return "Not plugged in, running on battery";
    case SDL_POWERSTATE_NO_BATTERY: return "Plugged in, no battery available";
    case SDL_POWERSTATE_CHARGING: return "Plugged in, charging battery";
    case SDL_POWERSTATE_CHARGED: return "Plugged in, battery charged";
    }
    return "Unknown";
}

SDL_PowerState GetPowerInfo(int* seconds, int* percent)
{
    return SDL_GetPowerInfo(seconds, percent);
}

} // namespace rad
