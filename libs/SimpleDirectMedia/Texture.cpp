#include "Texture.h"
#include "Renderer.h"
#include "rad/IO/Logging.h"

namespace sdl
{

Texture::Texture(Renderer* renderer) :
    m_renderer(renderer)
{
}

Texture::~Texture()
{
    if (m_handle)
    {
        Destroy();
    }
}

bool Texture::Create(Uint32 format, int access, int w, int h)
{
    m_handle = SDL_CreateTexture(m_renderer->GetHandle(),
        format, access, w, h);
    if (m_handle != nullptr)
    {
        m_format = format;
        m_access = access;
        m_width = w;
        m_height = h;
        return true;
    }
    else
    {
        LogGlobal(Error, "SDL_CreateTexture failed: %s", SDL_GetError());
        return false;
    }
}

bool Texture::CreateFromSurface(SDL_Surface* surface)
{
    m_handle = SDL_CreateTextureFromSurface(m_renderer->GetHandle(), surface);
    if (m_handle != nullptr)
    {
        int res = SDL_QueryTexture(m_handle, &m_format, &m_access, &m_width, &m_height);
        if (res != 0)
        {
            LogGlobal(Error, "SDL_QueryTexture failed: %s", SDL_GetError());
        }
        return true;
    }
    else
    {
        LogGlobal(Error, "SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        return false;
    }
}

void Texture::Destroy()
{
    SDL_DestroyTexture(m_handle);
    m_handle = nullptr;
}

int Texture::GetColorMultiplier(Uint8* r, Uint8* g, Uint8* b)
{
    int res = SDL_GetTextureColorMod(m_handle, r, g, b);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_GetTextureColorMod failed: %s", SDL_GetError());
    }
    return res;
}

int Texture::GetBlendMode(SDL_BlendMode* blendMode)
{
    int res = SDL_GetTextureBlendMode(m_handle, blendMode);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_GetTextureBlendMode failed: %s", SDL_GetError());
    }
    return res;
}

int Texture::GetAlphaMultiplier(Uint8* alpha)
{
    int res = SDL_GetTextureAlphaMod(m_handle, alpha);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_GetTextureAlphaMod failed: %s", SDL_GetError());
    }
    return res;
}

int Texture::SetColorMultiplier(Uint8 r, Uint8 g, Uint8 b)
{
    int res = SDL_SetTextureColorMod(m_handle, r, g, b);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_SetTextureColorMod failed: %s", SDL_GetError());
    }
    return res;
}

int Texture::SetBlendMode(SDL_BlendMode blendMode)
{
    int res = SDL_SetTextureBlendMode(m_handle, blendMode);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_SetTextureBlendMode failed: %s", SDL_GetError());
    }
    return res;
}

int Texture::SetAlphaMultiplier(Uint8 alpha)
{
    int res = SDL_SetTextureAlphaMod(m_handle, alpha);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_SetTextureAlphaMod failed: %s", SDL_GetError());
    }
    return res;
}

int Texture::Lock(const SDL_Rect* rect, void** pixels, int* pitch)
{
    int res = SDL_LockTexture(m_handle, rect, pixels, pitch);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_LockTexture failed: %s", SDL_GetError());
    }
    return res;
}

void Texture::Unlock()
{
    SDL_UnlockTexture(m_handle);
}

int Texture::Update(const SDL_Rect* rect, const void* pixels, int pitch)
{
    int res = SDL_UpdateTexture(m_handle, rect, pixels, pitch);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_UpdateTexture failed: %s", SDL_GetError());
    }
    return res;
}

} // namespace sdl
