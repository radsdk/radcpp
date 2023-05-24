#pragma once

#include "SDL2/SDL.h"

namespace sdl
{

class PerfCounter
{
public:
    enum class State
    {
        Stopped,
        Running,
    };

    PerfCounter()
    {
        m_frequency = SDL_GetPerformanceFrequency();
    }
    ~PerfCounter() {}

    // Returns the current counter value.
    Uint64 GetValue()
    {
        return SDL_GetPerformanceCounter();
    }

    // Get the count per second of the high resolution counter.
    // Returns a platform-specific count per second.
    Uint64 GetFrequency()
    {
        return m_frequency;
    }

    void Start()
    {
        assert(m_state == State::Stopped);
        m_start = GetValue();
        m_state = State::Running;
    }

    void Stop()
    {
        assert(m_state == State::Running);
        m_duration += GetValue() - m_start;
    }

    void Reset()
    {
        m_duration = 0;
        m_state = State::Stopped;
    }

    void Restart()
    {
        Reset();
        Start();
    }

    Uint64 GetElapsed() { return m_duration; }
    double GetElapsedInSeconds()
    {
        return double(m_duration) / double(m_frequency);
    }

private:
    State m_state = State::Stopped;
    // The count per second of the high resolution counter.
    Uint64 m_frequency = 0;
    Uint64 m_duration = 0;
    Uint64 m_start = 0;

}; // class PerfCounter

} // namespace sdl
