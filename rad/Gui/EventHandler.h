#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/RefCounted.h>
#include <SDL3/SDL_events.h>

namespace rad
{

class EventHandler : public rad::RefCounted<EventHandler>
{
public:
    EventHandler() = default;
    virtual ~EventHandler() {}
    virtual bool OnEvent(const SDL_Event& event) = 0;
    virtual void OnIdle() {}
}; // class EventHandler

} // namespace rad
