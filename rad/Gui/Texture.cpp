#include <rad/Gui/Texture.h>
#include <rad/Gui/Renderer.h>

namespace rad
{

rad::Ref<Texture> Texture::Create(
    rad::Ref<Renderer> renderer, SDL_PixelFormat format, int access, int w, int h)
{
    SDL_Texture* handle = SDL_CreateTexture(renderer->GetHandle(), format, access, w, h);
    if (handle)
    {
        return RAD_NEW Texture(std::move(renderer), handle);
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_CreateTexture failed: {}", SDL_GetError());
        return nullptr;
    }
}

rad::Ref<Texture> Texture::CreateFromSurface(rad::Ref<Renderer> renderer, SDL_Surface* surface)
{
    SDL_Texture* handle = SDL_CreateTextureFromSurface(renderer->GetHandle(), surface);
    if (handle)
    {
        return RAD_NEW Texture(std::move(renderer), handle);
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_CreateTexture failed: {}", SDL_GetError());
        return nullptr;
    }
}

Texture::Texture(rad::Ref<Renderer> renderer, SDL_Texture* handle) :
    m_renderer(std::move(renderer)),
    m_handle(handle)
{
    m_propID = SDL_GetTextureProperties(m_handle);
    if (m_propID == 0)
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetTextureProperties failed: {}", SDL_GetError());
    }
}

Texture::~Texture()
{
}

void Texture::Destroy()
{
    if (m_handle)
    {
        SDL_DestroyTexture(m_handle);
        m_handle = nullptr;
    }
}

const char* Texture::GetError()
{
    return SDL_GetError();
}

bool Texture::GetSize(float* w, float* h)
{
    int result = SDL_GetTextureSize(m_handle, w, h);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetTextureSize failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::SetColorMod(Uint8 r, Uint8 g, Uint8 b)
{
    int result = SDL_SetTextureColorMod(m_handle, r, g, b);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetTextureColorMod failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::SetColorMod(float r, float g, float b)
{
    int result = SDL_SetTextureColorModFloat(m_handle, r, g, b);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetTextureColorModFloat failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::GetColorMod(Uint8* r, Uint8* g, Uint8* b)
{
    int result = SDL_GetTextureColorMod(m_handle, r, g, b);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetTextureColorMod failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::GetColorMod(float* r, float* g, float* b)
{
    int result = SDL_GetTextureColorModFloat(m_handle, r, g, b);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetTextureColorModFloat failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::SetAlphaMod(Uint8 alpha)
{
    int result = SDL_SetTextureAlphaMod(m_handle, alpha);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetTextureAlphaMod failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::SetAlphaMod(float alpha)
{
    int result = SDL_SetTextureAlphaModFloat(m_handle, alpha);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetTextureAlphaModFloat failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::GetAlphaMod(Uint8* alpha)
{
    int result = SDL_GetTextureAlphaMod(m_handle, alpha);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetTextureAlphaMod failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::GetAlphaMod(float* alpha)
{
    int result = SDL_GetTextureAlphaModFloat(m_handle, alpha);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetTextureAlphaModFloat failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::SetBlendMode(SDL_BlendMode blendMode)
{
    int result = SDL_SetTextureBlendMode(m_handle, blendMode);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetTextureBlendMode failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::GetBlendMode(SDL_BlendMode* blendMode)
{
    int result = SDL_GetTextureBlendMode(m_handle, blendMode);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetTextureBlendMode failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::SetScaleMode(SDL_ScaleMode scaleMode)
{
    int result = SDL_SetTextureScaleMode(m_handle, scaleMode);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetTextureScaleMode failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::GetScaleMode(SDL_ScaleMode* scaleMode)
{
    int result = SDL_GetTextureScaleMode(m_handle, scaleMode);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetTextureScaleMode failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::Update(const SDL_Rect* rect, const void* pixels, int pitch)
{
    int result = SDL_UpdateTexture(m_handle, rect, pixels, pitch);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_UpdateTexture failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::UpdateYUV(const SDL_Rect* rect,
    const Uint8* Yplane, int Ypitch,
    const Uint8* Uplane, int Upitch,
    const Uint8* Vplane, int Vpitch)
{
    int result = SDL_UpdateYUVTexture(m_handle, rect,
        Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_UpdateYUVTexture failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::UpdateNV(const SDL_Rect* rect, const Uint8* Yplane, int Ypitch, const Uint8* UVplane, int UVpitch)
{
    int result = SDL_UpdateNVTexture(m_handle, rect,
        Yplane, Ypitch, UVplane, UVpitch);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_UpdateNVTexture failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::Lock(const SDL_Rect* rect, void** pixels, int* pitch)
{
    int result = SDL_LockTexture(m_handle, rect, pixels, pitch);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_LockTexture failed: {}", SDL_GetError());
        return false;
    }
}

bool Texture::LockToSurface(const SDL_Rect* rect, SDL_Surface** surface)
{
    int result = SDL_LockTextureToSurface(m_handle, rect, surface);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_LockTextureToSurface failed: {}", SDL_GetError());
        return false;
    }
}

void Texture::Unlock()
{
    SDL_UnlockTexture(m_handle);
}

} // namespace rad
