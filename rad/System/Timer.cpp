#include <rad/System/Timer.h>
#include <SDL3/SDL_init.h>
#include <rad/IO/Logging.h>

namespace rad
{

Uint64 GetPerfCounter()
{
    return SDL_GetPerformanceCounter();
}

Uint64 GetPerfCounterFrequency()
{
    return SDL_GetPerformanceFrequency();
}

void DelayMS(Uint32 ms)
{
    SDL_Delay(ms);
}

void DelayNS(Uint64 ns)
{
    SDL_DelayNS(ns);
}

Timer::Timer()
{
    SDL_InitSubSystem(SDL_INIT_TIMER);
}

Timer::~Timer()
{
    SDL_QuitSubSystem(SDL_INIT_TIMER);
}

bool Timer::StartMS(Uint32 interval, MSTimerCallback callback, void* userData)
{
    assert(m_id == 0);
    m_id = SDL_AddTimer(interval, callback, userData);
    if (m_id != 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetDefaultLogger(), err, "SDL_AddTimer failed: {}", SDL_GetError());
        return false;
    }
}

bool Timer::StartNS(Uint64 interval, NSTimerCallback callback, void* userData)
{
    assert(m_id == 0);
    m_id = SDL_AddTimerNS(interval, callback, userData);
    if (m_id != 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetDefaultLogger(), err, "SDL_AddTimerNS failed: {}", SDL_GetError());
        return false;
    }
}

void Timer::Stop()
{
    if (m_id != 0)
    {
        if (SDL_RemoveTimer(m_id) != 0)
        {
            RAD_LOG(GetDefaultLogger(), err, "SDL_RemoveTimer failed: {}", SDL_GetError());
        }
        m_id = 0;
    }
}

bool Timer::StartMS(Uint32 interval)
{
    return StartMS(interval, [](void* userData, SDL_TimerID timerID, Uint32 interval)->Uint32 {
        return reinterpret_cast<Timer*>(userData)->Callback(timerID, interval);
        }, this);
}

bool Timer::StartNS(Uint64 interval)
{
    return StartNS(interval, [](void* userData, SDL_TimerID timerID, Uint64 interval)->Uint64 {
        return reinterpret_cast<Timer*>(userData)->Callback(timerID, interval);
        }, this);
}

} // namespace rad
