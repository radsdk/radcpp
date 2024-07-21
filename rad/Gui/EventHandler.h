#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/RefCounted.h>
#include <SDL3/SDL_events.h>

namespace rad
{

class EventHandler : public rad::RefCounted<EventHandler>
{
public:
    EventHandler();
    virtual ~EventHandler();

    // Return true if the event has been dealt with, no need to send the event to other handlers.
    virtual bool OnEvent(const SDL_Event& event) = 0;
    // Expected to be called in SDL_AppIterate, to update states and draw a new frame.
    virtual void OnIdle() {}
}; // class EventHandler

} // namespace rad
