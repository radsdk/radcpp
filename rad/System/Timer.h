#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/RefCounted.h>
#include <SDL3/SDL_timer.h>

namespace rad
{

using MSTimerCallback = SDL_TimerCallback;
using NSTimerCallback = SDL_NSTimerCallback;

// Get the current value of the high resolution counter.
Uint64 GetPerfCounter();
// Get the frequency in Hz of the high resolution counter.
Uint64 GetPerfCounterFrequency();

// Wait a specified number of milliseconds before returning.
void DelayMS(Uint32 ms);
// Wait a specified number of nanoseconds before returning.
void DelayNS(Uint64 ns);

class Timer : public RefCounted<Timer>
{
public:
    Timer();
    virtual ~Timer();

    bool StartMS(Uint32 interval, MSTimerCallback callback, void* userData);
    bool StartNS(Uint64 interval, NSTimerCallback callback, void* userData);
    void Stop();

    SDL_TimerID GetID() const { return m_id; }

    virtual Uint32 Callback(SDL_TimerID timerID, Uint32 interval) { return 0; }
    virtual Uint64 Callback(SDL_TimerID timerID, Uint64 interval) { return 0; }

    bool StartMS(Uint32 interval);
    bool StartNS(Uint64 interval);

protected:
    SDL_TimerID m_id = 0;
}; // class Timer

} // namespace rad
