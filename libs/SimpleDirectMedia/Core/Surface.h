#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/RefCounted.h"
#include "rad/Core/Span.h"
#include "SDL2/SDL_surface.h"

namespace sdl
{

class Renderer;

class Surface : public rad::RefCounted<Surface>
{
public:
    Surface();
    Surface(SDL_Surface* handle);
    ~Surface();

    SDL_Surface* GetHandle() { return m_handle; }
    int GetWidth() const { return m_handle->w; }
    int GetHeight() const { return m_handle->h; }

    bool Create(Uint32 flags, int width, int height, int depth,
        Uint32 maskR, Uint32 maskG, Uint32 maskB, Uint32 maskA);
    bool CreateFrom(void* pixels, int width, int height, int depth, int pitch,
        Uint32 maskR, Uint32 maskG, Uint32 maskB, Uint32 maskA);
    void Destroy();

    bool Covert(const SDL_PixelFormat* format, Uint32 flags);

    int GetClipRect(SDL_Rect* rect);
    int GetColorKey(Uint32* key);
    // @param rect: the SDL_Rect structure representing the clipping rectangle, or NULL to disable clipping
    // Returns SDL_TRUE if the rectangle intersects the surface,
    // otherwise SDL_FALSE and blits will be completely clipped.
    SDL_bool SetClipRect(const SDL_Rect* rect);
    // Set the color key (transparent pixel) in a surface.
    int SetColorKey(int flag, Uint32 key);

    // Set the palette used by a surface.
    int SetSurfacePalette(SDL_Palette* palette);

    // Set the RLE acceleration hint for a surface.
    // @param flag: 0 to disable, non-zero to enable RLE acceleration
    int SetRLE(int flag);

    int GetColorMultiplier(Uint8* r, Uint8* g, Uint8* b);
    int GetBlendMode(SDL_BlendMode* blendMode);
    int GetAlphaMultiplier(Uint8* alpha);

    int SetColorMultiplier(Uint8 r, Uint8 g, Uint8 b);
    int SetBlendMode(SDL_BlendMode blendMode);
    int SetAlphaMultiplier(Uint8 alpha);

    // Set up a surface for directly accessing the pixels.
    int Lock();
    // Release a surface after directly accessing the pixels.
    void Unlock();

    bool RequireLockBeforeAccess();

    int FillRect(const SDL_Rect* rect, Uint32 color);
    int FillRects(const SDL_Rect* rects, int count, Uint32 color);

    int LowerBlit(SDL_Rect* srcRect, Surface* dst, SDL_Rect* dstRect);
    // Use this function to perform a scaled surface copy to a destination surface.
    int BlitScaled(SDL_Rect* srcRect, Surface* dst, SDL_Rect* dstRect);

private:
    SDL_Surface* m_handle = nullptr;

}; // class Surface

} // namespace sdl
