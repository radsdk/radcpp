#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/RefCounted.h"
#include "rad/Core/Span.h"
#include "SDL2/SDL_render.h"

namespace sdl
{

class Renderer;

class Texture : public rad::RefCounted<Texture>
{
public:
    Texture(Renderer* renderer);
    ~Texture();

    SDL_Texture* GetHandle() { return m_handle; }

    // @param format SDL_PixelFormatEnum: https://wiki.libsdl.org/SDL2/SDL_PixelFormatEnum
    // @param access SDL_TextureAccess
    // SDL_TEXTUREACCESS_STATIC: changes rarely, not lockable
    // SDL_TEXTUREACCESS_STREAMING: changes frequently, lockable
    // SDL_TEXTUREACCESS_TARGET: can be used as a render target
    bool Create(Uint32 format, int access, int w, int h);
    bool CreateFromSurface(SDL_Surface* surface);
    void Destroy();

    int GetColorMultiplier(Uint8* r, Uint8* g, Uint8* b);
    int GetBlendMode(SDL_BlendMode* blendMode);
    int GetAlphaMultiplier(Uint8* alpha);

    int SetColorMultiplier(Uint8 r, Uint8 g, Uint8 b);
    int SetBlendMode(SDL_BlendMode blendMode);
    int SetAlphaMultiplier(Uint8 alpha);

    // Lock a portion of the texture for write-only pixel access.
    int Lock(const SDL_Rect* rect, void** pixels, int* pitch);
    // Unlock a texture, uploading the changes to video memory, if needed.
    void Unlock();
    // Update the given texture rectangle with new pixel data.
    int Update(const SDL_Rect* rect, const void* pixels, int pitch);

private:
    rad::Ref<Renderer> m_renderer;
    SDL_Texture* m_handle = nullptr;
    Uint32 m_format = 0;
    int m_access = 0;
    int m_width = 0;
    int m_height = 0;

}; // class Texture

} // namespace sdl
