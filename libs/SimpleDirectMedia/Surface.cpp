#include "Surface.h"
#include "rad/IO/Logging.h"

namespace sdl
{

Surface::Surface()
{
}

Surface::Surface(SDL_Surface* handle) :
    m_handle(handle)
{
}

Surface::~Surface()
{
    if (m_handle)
    {
        Destroy();
    }
}

bool Surface::Create(Uint32 flags, int width, int height, int depth,
    Uint32 maskR, Uint32 maskG, Uint32 maskB, Uint32 maskA)
{
    m_handle = SDL_CreateRGBSurface(flags, width, height, depth,
        maskR, maskG, maskB, maskA);
    if (m_handle != nullptr)
    {
        return true;
    }
    else
    {
        LogGlobal(Error, "SDL_CreateRGBSurface failed: %s", SDL_GetError());
        return false;
    }
}

bool Surface::CreateFrom(void* pixels, int width, int height, int depth, int pitch,
    Uint32 maskR, Uint32 maskG, Uint32 maskB, Uint32 maskA)
{
    m_handle = SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch,
        maskR, maskG, maskB, maskA);
    if (m_handle != nullptr)
    {
        return true;
    }
    else
    {
        LogGlobal(Error, "SDL_CreateRGBSurface failed: %s", SDL_GetError());
        return false;
    }
}

void Surface::Destroy()
{
    SDL_FreeSurface(m_handle);
    m_handle = nullptr;
}

bool Surface::Covert(const SDL_PixelFormat* format, Uint32 flags)
{
    SDL_Surface* newSurface = SDL_ConvertSurface(m_handle, format, flags);
    if (newSurface != nullptr)
    {
        m_handle = newSurface;
        return true;
    }
    else
    {
        LogGlobal(Error, "SDL_ConvertSurface failed: %s", SDL_GetError());
        return false;
    }
}

int Surface::GetClipRect(SDL_Rect* rect)
{
    SDL_GetClipRect(m_handle, rect);
    return 0;
}

int Surface::GetColorKey(Uint32* key)
{
    int res = SDL_GetColorKey(m_handle, key);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_GetColorKey failed: %s", SDL_GetError());
    }
    return res;
}

SDL_bool Surface::SetClipRect(const SDL_Rect* rect)
{
    return SDL_SetClipRect(m_handle, rect);
}

int Surface::SetColorKey(int flag, Uint32 key)
{
    int res = SDL_SetColorKey(m_handle, flag, key);
    if (res != 0)
    {
        LogGlobal(Error, "SetColorKey failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::SetSurfacePalette(SDL_Palette* palette)
{
    int res = SDL_SetSurfacePalette(m_handle, palette);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_SetSurfacePalette failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::SetRLE(int flag)
{
    int res = SDL_SetSurfaceRLE(m_handle, flag);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_SetSurfaceRLE failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::GetColorMultiplier(Uint8* r, Uint8* g, Uint8* b)
{
    int res = SDL_GetSurfaceColorMod(m_handle, r, g, b);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_GetSurfaceColorMod failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::GetBlendMode(SDL_BlendMode* blendMode)
{
    int res = SDL_GetSurfaceBlendMode(m_handle, blendMode);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_GetSurfaceBlendMode failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::GetAlphaMultiplier(Uint8* alpha)
{
    int res = SDL_GetSurfaceAlphaMod(m_handle, alpha);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_GetSurfaceAlphaMod failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::SetColorMultiplier(Uint8 r, Uint8 g, Uint8 b)
{
    int res = SDL_SetSurfaceColorMod(m_handle, r, g, b);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_SetSurfaceColorMod failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::SetBlendMode(SDL_BlendMode blendMode)
{
    int res = SDL_SetSurfaceBlendMode(m_handle, blendMode);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_SetSurfaceBlendMode failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::SetAlphaMultiplier(Uint8 alpha)
{
    int res = SDL_SetSurfaceAlphaMod(m_handle, alpha);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_SetSurfaceAlphaMod failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::Lock()
{
    int res = SDL_LockSurface(m_handle);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_LockSurface failed: %s", SDL_GetError());
    }
    return res;
}

void Surface::Unlock()
{
    SDL_UnlockSurface(m_handle);
}

bool Surface::RequireLockBeforeAccess()
{
    return SDL_MUSTLOCK(m_handle);
}

int Surface::FillRect(const SDL_Rect* rect, Uint32 color)
{
    int res = SDL_FillRect(m_handle, rect, color);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_FillRect failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::FillRects(const SDL_Rect* rects, int count, Uint32 color)
{
    int res = SDL_FillRects(m_handle, rects, count, color);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_FillRects failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::LowerBlit(SDL_Rect* srcRect, Surface* dst, SDL_Rect* dstRect)
{
    int res = SDL_LowerBlit(m_handle, srcRect, dst->GetHandle(), dstRect);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_LowerBlit failed: %s", SDL_GetError());
    }
    return res;
}

int Surface::BlitScaled(SDL_Rect* srcRect, Surface* dst, SDL_Rect* dstRect)
{
    int res = SDL_BlitScaled(m_handle, srcRect, dst->GetHandle(), dstRect);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_BlitScaled failed: %s", SDL_GetError());
    }
    return res;
}

} // namespace sdl
