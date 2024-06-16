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
    Texture(rad::Ref<Renderer> renderer);
    ~Texture();

    bool Create(SDL_PixelFormatEnum format, int access, int w, int h);
    bool CreateFromSurface(SDL_Surface* surface);
    void Destroy();

    SDL_Texture* GetHandle() { return m_handle; }

    int Query(SDL_PixelFormatEnum* format, int* access, int* w, int* h);
    int SetColorMod(Uint8 r, Uint8 g, Uint8 b);
    int SetColorMod(float r, float g, float b);
    int GetColorMod(Uint8* r, Uint8* g, Uint8* b);
    int GetColorMod(float* r, float* g, float* b);
    int SetAlphaMod(Uint8 alpha);
    int SetAlphaMod(float alpha);
    int GetAlphaMod(Uint8* alpha);
    int GetAlphaMod(float* alpha);
    int SetBlendMode(SDL_BlendMode blendMode);
    int GetBlendMode(SDL_BlendMode* blendMode);
    int SetScaleMode(SDL_ScaleMode scaleMode);
    int GetScaleMode(SDL_ScaleMode* scaleMode);

    int Update(const SDL_Rect* rect, const void* pixels, int pitch);
    int UpdateYUV(const SDL_Rect* rect,
        const Uint8* Yplane, int Ypitch,
        const Uint8* Uplane, int Upitch,
        const Uint8* Vplane, int Vpitch);
    int UpdateNV(const SDL_Rect* rect,
        const Uint8* Yplane, int Ypitch,
        const Uint8* UVplane, int UVpitch);

    int Lock(const SDL_Rect* rect,
        void** pixels, int* pitch);
    int LockToSurface(const SDL_Rect* rect,
        SDL_Surface** surface);
    void Unlock();

private:
    rad::Ref<Renderer> m_renderer;
    SDL_Texture* m_handle = nullptr;
    SDL_PropertiesID m_propID = 0;

}; // class Texture

} // namespace rad
