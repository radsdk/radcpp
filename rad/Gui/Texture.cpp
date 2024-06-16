#include <rad/Gui/Texture.h>
#include <rad/Gui/Renderer.h>

namespace rad
{

Texture::Texture(rad::Ref<Renderer> renderer) :
    m_renderer(renderer)
{
}

Texture::~Texture()
{
}

bool Texture::Create(SDL_PixelFormatEnum format, int access, int w, int h)
{
    auto logger = rad::GetGuiLogger();
    m_handle = SDL_CreateTexture(m_renderer->GetHandle(), format, access, w, h);
    if (m_handle)
    {
        m_propID = SDL_GetTextureProperties(m_handle);
        if (m_propID == 0)
        {
            RAD_LOG(logger, err, "SDL_GetTextureProperties failed: {}", SDL_GetError());
        }
        return true;
    }
    else
    {
        RAD_LOG(logger, err, "SDL_CreateTexture failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::CreateFromSurface(SDL_Surface* surface)
{
    auto logger = rad::GetGuiLogger();
    m_handle = SDL_CreateTextureFromSurface(m_renderer->GetHandle(), surface);
    if (m_handle)
    {
        if (m_propID == 0)
        {
            RAD_LOG(logger, err, "SDL_GetTextureProperties failed: {}", SDL_GetError());
        }
        return true;
    }
    else
    {
        RAD_LOG(logger, err, "SDL_CreateTexture failed: {}", SDL_GetError());
        return false;
    }
}

void Texture::Destroy()
{
    if (m_handle)
    {
        SDL_DestroyTexture(m_handle);
        m_handle = nullptr;
    }
}

int Texture::Query(SDL_PixelFormatEnum* format, int* access, int* w, int* h)
{
    return SDL_QueryTexture(m_handle, format, access, w, h);
}

int Texture::SetColorMod(Uint8 r, Uint8 g, Uint8 b)
{
    return SDL_SetTextureColorMod(m_handle, r, g, b);
}

int Texture::SetColorMod(float r, float g, float b)
{
    return SDL_SetTextureColorModFloat(m_handle, r, g, b);
}

int Texture::GetColorMod(Uint8* r, Uint8* g, Uint8* b)
{
    return SDL_GetTextureColorMod(m_handle, r, g, b);
}

int Texture::GetColorMod(float* r, float* g, float* b)
{
    return SDL_GetTextureColorModFloat(m_handle, r, g, b);
}

int Texture::SetAlphaMod(Uint8 alpha)
{
    return SDL_SetTextureAlphaMod(m_handle, alpha);
}

int Texture::SetAlphaMod(float alpha)
{
    return SDL_SetTextureAlphaModFloat(m_handle, alpha);
}

int Texture::GetAlphaMod(Uint8* alpha)
{
    return SDL_GetTextureAlphaMod(m_handle, alpha);
}

int Texture::GetAlphaMod(float* alpha)
{
    return SDL_GetTextureAlphaModFloat(m_handle, alpha);
}

int Texture::SetBlendMode(SDL_BlendMode blendMode)
{
    return SDL_SetTextureBlendMode(m_handle, blendMode);
}

int Texture::GetBlendMode(SDL_BlendMode* blendMode)
{
    return SDL_GetTextureBlendMode(m_handle, blendMode);
}

int Texture::SetScaleMode(SDL_ScaleMode scaleMode)
{
    return SDL_SetTextureScaleMode(m_handle, scaleMode);
}

int Texture::GetScaleMode(SDL_ScaleMode* scaleMode)
{
    return SDL_GetTextureScaleMode(m_handle, scaleMode);
}

int Texture::Update(const SDL_Rect* rect, const void* pixels, int pitch)
{
    return SDL_UpdateTexture(m_handle, rect, pixels, pitch);
}

int Texture::UpdateYUV(const SDL_Rect* rect,
    const Uint8* Yplane, int Ypitch,
    const Uint8* Uplane, int Upitch,
    const Uint8* Vplane, int Vpitch)
{
    return SDL_UpdateYUVTexture(m_handle, rect,
        Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
}

int Texture::UpdateNV(const SDL_Rect* rect, const Uint8* Yplane, int Ypitch, const Uint8* UVplane, int UVpitch)
{
    return SDL_UpdateNVTexture(m_handle, rect,
        Yplane, Ypitch, UVplane, UVpitch);
}

int Texture::Lock(const SDL_Rect* rect, void** pixels, int* pitch)
{
    return SDL_LockTexture(m_handle, rect, pixels, pitch);
}

int Texture::LockToSurface(const SDL_Rect* rect, SDL_Surface** surface)
{
    return SDL_LockTextureToSurface(m_handle, rect, surface);
}

void Texture::Unlock()
{
    SDL_UnlockTexture(m_handle);
}

} // namespace rad
