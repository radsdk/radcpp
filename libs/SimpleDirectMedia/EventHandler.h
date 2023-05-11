#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/RefCounted.h"
#include "SDL2/SDL.h"

namespace sdl
{

class EventHandler : public rad::RefCounted<EventHandler>
{
public:
    EventHandler() = default;
    virtual ~EventHandler() {}
    virtual bool OnEvent(const SDL_Event& event) = 0;
    virtual void OnIdle() {}
}; // class EventHandler

} // namespace sdl
