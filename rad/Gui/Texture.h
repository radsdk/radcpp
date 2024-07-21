#pragma once

#include <rad/Core/RefCounted.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

namespace rad
{

class Renderer;

class Texture : public RefCounted<Texture>
{
public:
    static rad::Ref<Texture> Create(rad::Ref<Renderer> renderer,
        SDL_PixelFormat format, int access, int w, int h);
    static rad::Ref<Texture> CreateFromSurface(rad::Ref<Renderer> renderer, SDL_Surface* surface);

    Texture(rad::Ref<Renderer> renderer, SDL_Texture* handle);
    ~Texture();
    void Destroy();

    SDL_Texture* GetHandle() { return m_handle; }
    const char* GetError();

    bool GetSize(float* w, float* h);
    bool SetColorMod(Uint8 r, Uint8 g, Uint8 b);
    bool SetColorMod(float r, float g, float b);
    bool GetColorMod(Uint8* r, Uint8* g, Uint8* b);
    bool GetColorMod(float* r, float* g, float* b);
    bool SetAlphaMod(Uint8 alpha);
    bool SetAlphaMod(float alpha);
    bool GetAlphaMod(Uint8* alpha);
    bool GetAlphaMod(float* alpha);
    bool SetBlendMode(SDL_BlendMode blendMode);
    bool GetBlendMode(SDL_BlendMode* blendMode);
    bool SetScaleMode(SDL_ScaleMode scaleMode);
    bool GetScaleMode(SDL_ScaleMode* scaleMode);

    bool Update(const SDL_Rect* rect, const void* pixels, int pitch);
    bool UpdateYUV(const SDL_Rect* rect,
        const Uint8* Yplane, int Ypitch,
        const Uint8* Uplane, int Upitch,
        const Uint8* Vplane, int Vpitch);
    bool UpdateNV(const SDL_Rect* rect,
        const Uint8* Yplane, int Ypitch,
        const Uint8* UVplane, int UVpitch);

    bool Lock(const SDL_Rect* rect,
        void** pixels, int* pitch);
    bool LockToSurface(const SDL_Rect* rect,
        SDL_Surface** surface);
    void Unlock();

private:
    rad::Ref<Renderer> m_renderer;
    SDL_Texture* m_handle = nullptr;
    SDL_PropertiesID m_propID = 0;

}; // class Texture

} // namespace rad
