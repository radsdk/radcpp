#include "DirectMedia/Core/Timer.h"
#include "rad/IO/Logging.h"

namespace sdl
{

Timer::Timer()
{
}

Timer::~Timer()
{
    if (m_id != 0)
    {
        Stop();
    }
}

SDL_TimerID Timer::Start(Uint32 interval, SDL_TimerCallback callback, void* param)
{
    m_id = SDL_AddTimer(interval, callback, param);
    if (m_id == 0)
    {
        LogGlobal(Error, "SDL_AddTimer failed: %s", SDL_GetError());
    }
    return m_id;
}

void Timer::Stop()
{
    SDL_RemoveTimer(m_id);
    m_id = 0;
}

} // namespace sdl
