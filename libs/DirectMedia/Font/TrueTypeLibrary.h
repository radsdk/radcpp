#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/RefCounted.h"
#include "rad/Core/Span.h"
#include "rad/Core/String.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

namespace sdl
{

class TrueTypeFont;

class TrueTypeLibrary : public rad::RefCounted<TrueTypeLibrary>
{
public:
    TrueTypeLibrary();
    ~TrueTypeLibrary();

    rad::Ref<TrueTypeFont> CreateTrueTypeFont();

private:
    bool Init();
    void Destroy();

    const SDL_version* m_pVersion = nullptr;

}; // class TrueTypeLibrary

} // namespace sdl
