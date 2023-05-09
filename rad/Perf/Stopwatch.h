#pragma once

#include "rad/Core/Common.h"
#include <chrono>

NAMESPACE_RAD_BEGIN

class Stopwatch
{
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    enum class State
    {
        Stopped,
        Running,
    };

    Stopwatch() { Reset(); }
    ~Stopwatch() {}

    State GetState() { return m_state; }
    bool IsRunning() { return (m_state == State::Running); }

    void Start()
    {
        assert(m_state == State::Stopped);
        m_start = Clock::now();
        m_state = State::Running;
    }

    void Stop()
    {
        assert(m_state == State::Running);
        m_duration += Clock::now() - m_start;
    }

    void Reset()
    {
        m_duration = Clock::duration::zero();
        m_state = State::Stopped;
    }

    void Restart()
    {
        Reset();
        Start();
    }

    template<typename Duration = std::chrono::seconds>
    auto GetElapsed() const
    {
        std::chrono::duration_cast<Duration>(m_duration).count();
    }

    long long GetElapsedInNanoSeconds() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(m_duration).count();
    }

    long long GetElapsedInMacroSeconds() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(m_duration).count();
    }

    long long GetElapsedInMilliSeconds() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_duration).count();
    }

    long long GetElapsedInSeconds() const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(m_duration).count();
    }

private:
    State m_state = State::Stopped;
    Clock::duration m_duration;
    TimePoint m_start;

}; // class Stopwatch

NAMESPACE_RAD_END
