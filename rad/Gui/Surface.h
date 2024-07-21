#pragma once

#include <rad/Core/RefCounted.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

namespace rad
{

class Surface : public RefCounted<Surface>
{
public:
    static rad::Ref<Surface> Create(int width, int height, SDL_PixelFormat format);
    static rad::Ref<Surface> CreateFormPixels(
        int width, int height, SDL_PixelFormat format, void* pixels, int pitch);
    static rad::Ref<Surface> CreateFromBMP(SDL_IOStream* src, SDL_bool closeio);
    static rad::Ref<Surface> CreateFromBMP(const char* file);

    Surface(SDL_Surface* handle, bool isManaged);
    ~Surface();
    void Destroy();

    SDL_Surface* GetHandle() { return m_handle; }
    const char* GetError();

    bool SaveBMP(SDL_IOStream* dst, SDL_bool closeio);
    bool SaveBMP(const char* file);

    bool SetColorSpace(SDL_Colorspace colorspace);
    SDL_Colorspace GetColorSpace(SDL_Colorspace* colorspace);
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
    rad::Ref<Surface> Convert(SDL_PixelFormat format);
    rad::Ref<Surface> Convert(
        SDL_PixelFormat format, SDL_Palette* palette, SDL_Colorspace colorspace, SDL_PropertiesID props);

    bool PremultiplyAlpha(bool linear);
    bool Clear(float r, float g, float b, float a);

    bool FillRect(const SDL_Rect* rect, Uint32 color);
    bool FillRects(const SDL_Rect* rects, int count, Uint32 color);

    static bool Blit(Surface* src, const SDL_Rect* srcRect, Surface* dst, SDL_Rect* dstRect);
    static bool BlitUnchecked(Surface* src, const SDL_Rect* srcRect, Surface* dst, const SDL_Rect* dstRect);
    static bool BlitScaled(Surface* src, const SDL_Rect* srcRect,
        Surface* dst, SDL_Rect* dstRect, SDL_ScaleMode scaleMode);
    static bool BlitUncheckedScaled(Surface* src, const SDL_Rect* srcRect,
        Surface* dst, SDL_Rect* dstRect, SDL_ScaleMode scaleMode);
    static bool BlitTiled(Surface* src, const SDL_Rect* srcRect, Surface* dst, SDL_Rect* dstRect);
    static bool BlitTiledWithScale(Surface* src, const SDL_Rect* srcRect,
        float scale, SDL_ScaleMode scaleMode,
        Surface* dst, const SDL_Rect* dstRect);
    static bool Blit9Grid(Surface* src, const SDL_Rect* srcRect,
        int cornerSize, float scale, SDL_ScaleMode scaleMode,
        SDL_Surface* dst, const SDL_Rect* dstRect);

    bool ReadPixel(int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a);

private:
    SDL_Surface* m_handle;
    bool m_isManaged;
    SDL_PropertiesID m_propID = 0;

}; // class Surface

} // namespace rad
