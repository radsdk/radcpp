#pragma once

#include <rad/Core/RefCounted.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

namespace rad
{

class Surface : public RefCounted<Surface>
{
public:
    static rad::Ref<Surface> Create(int width, int height, SDL_PixelFormatEnum format);
    static rad::Ref<Surface> CreateFormPixels(void* pixels, int width, int height, int pitch, SDL_PixelFormatEnum format);
    static rad::Ref<Surface> CreateFromBMP(SDL_IOStream* src, SDL_bool closeio);
    static rad::Ref<Surface> CreateFromBMP(const char* file);

    Surface(SDL_Surface* handle);
    ~Surface();
    void Destroy();

    SDL_Surface* GetHandle() { return m_handle; }
    const char* GetError();

    bool SaveBMP(SDL_IOStream* dst, SDL_bool closeio);
    bool SaveBMP(const char* file);

    bool SetColorSpace(SDL_Colorspace colorspace);
    bool GetColorSpace(SDL_Colorspace* colorspace);
    bool SetPalette(SDL_Palette* palette);

    bool Lock();
    void Unlock();

    bool SetRLE(int flag);
    bool HasRLE();
    bool SetColorKey(int flag, Uint32 key);
    bool HasColorKey();
    bool GetColorKey(Uint32* key);
    bool SetColorMod(Uint8 r, Uint8 g, Uint8 b);
    bool GetColorMod(Uint8* r, Uint8* g, Uint8* b);
    bool SetAlphaMode(Uint8 alpha);
    bool GetAlphaMode(Uint8* alpha);
    bool SetBlendMode(SDL_BlendMode blendMode);
    bool GetBlendMode(SDL_BlendMode* blendMode);
    bool SetClipRect(const SDL_Rect* rect);
    bool DisableClipping();
    bool GetClipRect(SDL_Rect* rect);
    bool SetClipRect(const SDL_Rect& rect) { return SetClipRect(&rect); }
    bool GetClipRect(SDL_Rect& rect) { return GetClipRect(&rect); }

    bool Flip(SDL_FlipMode flip);
    rad::Ref<Surface> Duplicate();
    rad::Ref<Surface> Convert(const SDL_PixelFormat* format);
    rad::Ref<Surface> Convert(SDL_PixelFormatEnum format);
    rad::Ref<Surface> Convert(SDL_PixelFormatEnum format, SDL_Colorspace colorspace, SDL_PropertiesID propID);

    bool FillRect(const SDL_Rect* rect, Uint32 color);
    bool FillRects(const SDL_Rect* rects, int count, Uint32 color);

    static bool Blit(Surface* src, const SDL_Rect* srcRect, Surface* dst, SDL_Rect* dstRect);
    static bool BlitUnchecked(Surface* src, const SDL_Rect* srcRect, Surface* dst, const SDL_Rect* dstRect);
    static bool BlitScaledSoft(Surface* src, const SDL_Rect* srcRect,
        Surface* dst, const SDL_Rect* dstRect, SDL_ScaleMode scaleMode);
    static bool BlitScaled(Surface* src, const SDL_Rect* srcRect,
        Surface* dst, SDL_Rect* dstRect, SDL_ScaleMode scaleMode);
    static bool BlitUncheckedScaled(Surface* src, const SDL_Rect* srcRect,
        Surface* dst, SDL_Rect* dstRect, SDL_ScaleMode scaleMode);

    bool ReadPixel(int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a);

private:
    SDL_Surface* m_handle;
    SDL_PropertiesID m_propID = 0;

}; // class Surface

} // namespace rad
