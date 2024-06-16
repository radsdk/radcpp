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

    int SaveBMP(SDL_IOStream* dst, SDL_bool closeio);
    int SaveBMP(const char* file);

    int SetColorSpace(SDL_Colorspace colorspace);
    int GetColorSpace(SDL_Colorspace* colorspace);
    int SetPalette(SDL_Palette* palette);

    int Lock();
    void Unlock();

    int SetRLE(int flag);
    bool HasRLE();
    int SetColorKey(int flag, Uint32 key);
    bool HasColorKey();
    int GetColorKey(Uint32* key);
    int SetColorMod(Uint8 r, Uint8 g, Uint8 b);
    int GetColorMod(Uint8* r, Uint8* g, Uint8* b);
    int SetAlphaMode(Uint8 alpha);
    int GetAlphaMode(Uint8* alpha);
    int SetBlendMode(SDL_BlendMode blendMode);
    int GetBlendMode(SDL_BlendMode* blendMode);
    int SetClipRect(const SDL_Rect* rect);
    int GetClipRect(SDL_Rect* rect);

    int Flip(SDL_FlipMode flip);
    rad::Ref<Surface> Duplicate();
    rad::Ref<Surface> Convert(const SDL_PixelFormat* format);
    rad::Ref<Surface> Convert(SDL_PixelFormatEnum format);
    rad::Ref<Surface> Convert(SDL_PixelFormatEnum format, SDL_Colorspace colorspace, SDL_PropertiesID propID);

    int FillRect(const SDL_Rect* rect, Uint32 color);
    int FillRects(const SDL_Rect* rects, int count, Uint32 color);

    static int Blit(Surface* src, const SDL_Rect* srcRect, Surface* dst, SDL_Rect* dstRect);
    static int BlitUnchecked(Surface* src, const SDL_Rect* srcRect, Surface* dst, const SDL_Rect* dstRect);
    static int BlitScaledSoft(Surface* src, const SDL_Rect* srcRect,
        Surface* dst, const SDL_Rect* dstRect, SDL_ScaleMode scaleMode);
    static int BlitScaled(Surface* src, const SDL_Rect* srcRect,
        Surface* dst, SDL_Rect* dstRect, SDL_ScaleMode scaleMode);
    static int BlitUncheckedScaled(Surface* src, const SDL_Rect* srcRect,
        Surface* dst, SDL_Rect* dstRect, SDL_ScaleMode scaleMode);

    int ReadPixel(int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a);

private:
    SDL_Surface* m_handle;
    SDL_PropertiesID m_propID = 0;

}; // class Surface

} // namespace rad
