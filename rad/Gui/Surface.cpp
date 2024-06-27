#include <rad/Core/MemoryDebug.h>
#include <rad/Gui/Surface.h>
#include <rad/Gui/Window.h>

namespace rad
{

rad::Ref<Surface> Surface::Create(int width, int height, SDL_PixelFormatEnum format)
{
    SDL_Surface* handle = SDL_CreateSurface(width, height, format);
    if (handle)
    {
        return RAD_NEW Surface(handle);
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_CreateSurface failed: {}", SDL_GetError());
        return nullptr;
    }
}

rad::Ref<Surface> Surface::CreateFormPixels(void* pixels, int width, int height, int pitch, SDL_PixelFormatEnum format)
{
    SDL_Surface* handle = SDL_CreateSurfaceFrom(pixels, width, height, pitch, format);
    if (handle)
    {
        return RAD_NEW Surface(handle);
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_CreateSurfaceFrom failed: {}", SDL_GetError());
        return nullptr;
    }
}

rad::Ref<Surface> Surface::CreateFromBMP(SDL_IOStream* src, SDL_bool closeio)
{
    SDL_Surface* handle = SDL_LoadBMP_IO(src, closeio);
    if (handle)
    {
        return RAD_NEW Surface(handle);
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_LoadBMP_IO failed: {}", SDL_GetError());
        return nullptr;
    }
}

rad::Ref<Surface> Surface::CreateFromBMP(const char* file)
{
    SDL_Surface* handle = SDL_LoadBMP(file);
    if (handle)
    {
        return RAD_NEW Surface(handle);
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_LoadBMP_IO failed: {}", SDL_GetError());
        return nullptr;
    }
}

Surface::Surface(SDL_Surface* handle) :
    m_handle(handle)
{
    m_propID = SDL_GetSurfaceProperties(m_handle);
    if (m_propID == 0)
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetSurfaceProperties failed: {}", SDL_GetError());
    }
}

Surface::~Surface()
{
    Destroy();
}

void Surface::Destroy()
{
    if (m_handle)
    {
        SDL_DestroySurface(m_handle);
        m_handle = nullptr;
    }
}

const char* Surface::GetError()
{
    return SDL_GetError();
}

bool Surface::SaveBMP(SDL_IOStream* dst, SDL_bool closeio)
{
    if (SDL_SaveBMP_IO(m_handle, dst, closeio) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SaveBMP_IO failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::SaveBMP(const char* file)
{
    if (SDL_SaveBMP(m_handle, file) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SaveBMP(file='{}') failed: {}",
            file, SDL_GetError());
        return false;
    }
}

bool Surface::SetColorSpace(SDL_Colorspace colorspace)
{
    if (SDL_SetSurfaceColorspace(m_handle, colorspace) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetSurfaceColorspace failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::GetColorSpace(SDL_Colorspace* colorspace)
{
    if (SDL_GetSurfaceColorspace(m_handle, colorspace) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetSurfaceColorspace failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::SetPalette(SDL_Palette* palette)
{
    if (SDL_SetSurfacePalette(m_handle, palette) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetSurfacePalette failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::Lock()
{
    if (SDL_LockSurface(m_handle) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_LockSurface failed: {}", SDL_GetError());
        return false;
    }
}

void Surface::Unlock()
{
    SDL_UnlockSurface(m_handle);
}

bool Surface::SetRLE(int flag)
{
    if (SDL_SetSurfaceRLE(m_handle, flag) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetSurfaceRLE failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::HasRLE()
{
    return (SDL_SurfaceHasRLE(m_handle) == SDL_TRUE);
}

bool Surface::SetColorKey(int flag, Uint32 key)
{
    if (SDL_SetSurfaceColorKey(m_handle, flag, key) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetSurfaceColorKey failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::HasColorKey()
{
    return (SDL_SurfaceHasColorKey(m_handle) == SDL_TRUE);
}

bool Surface::GetColorKey(Uint32* key)
{
    if (SDL_GetSurfaceColorKey(m_handle, key) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetSurfaceColorKey failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::SetColorMod(Uint8 r, Uint8 g, Uint8 b)
{
    if (SDL_SetSurfaceColorMod(m_handle, r, g, b) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetSurfaceColorMod failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::GetColorMod(Uint8* r, Uint8* g, Uint8* b)
{
    if (SDL_GetSurfaceColorMod(m_handle, r, g, b) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetSurfaceColorMod failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::SetAlphaMode(Uint8 alpha)
{
    if (SDL_SetSurfaceAlphaMod(m_handle, alpha) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetSurfaceAlphaMod failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::GetAlphaMode(Uint8* alpha)
{
    if (SDL_GetSurfaceAlphaMod(m_handle, alpha) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetSurfaceAlphaMod failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::SetBlendMode(SDL_BlendMode blendMode)
{
    if (SDL_SetSurfaceBlendMode(m_handle, blendMode) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetSurfaceBlendMode failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::GetBlendMode(SDL_BlendMode* blendMode)
{
    if (SDL_GetSurfaceBlendMode(m_handle, blendMode) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetSurfaceBlendMode failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::SetClipRect(const SDL_Rect* rect)
{
    if (SDL_SetSurfaceClipRect(m_handle, rect) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetSurfaceClipRect failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::GetClipRect(SDL_Rect* rect)
{
    if (SDL_GetSurfaceClipRect(m_handle, rect) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetSurfaceClipRect failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::Flip(SDL_FlipMode flip)
{
    if (SDL_FlipSurface(m_handle, flip) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_FlipSurface failed: {}", SDL_GetError());
        return false;
    }
}

rad::Ref<Surface> Surface::Duplicate()
{
    SDL_Surface* handle = SDL_DuplicateSurface(m_handle);
    if (handle != nullptr)
    {
        return rad::Ref<Surface>(RAD_NEW Surface(handle));
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_DuplicateSurface failed: {}", SDL_GetError());
        return nullptr;
    }
}

rad::Ref<Surface> Surface::Convert(const SDL_PixelFormat* format)
{
    SDL_Surface* handle = SDL_ConvertSurface(m_handle, format);
    if (handle != nullptr)
    {
        return rad::Ref<Surface>(RAD_NEW Surface(handle));
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_ConvertSurface failed: {}", SDL_GetError());
        return nullptr;
    }
}

rad::Ref<Surface> Surface::Convert(SDL_PixelFormatEnum format)
{
    SDL_Surface* handle = SDL_ConvertSurfaceFormat(m_handle, format);
    if (handle != nullptr)
    {
        return rad::Ref<Surface>(RAD_NEW Surface(handle));
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_ConvertSurfaceFormat failed: {}", SDL_GetError());
        return nullptr;
    }
}

rad::Ref<Surface> Surface::Convert(SDL_PixelFormatEnum format, SDL_Colorspace colorspace,
    SDL_PropertiesID propID)
{
    SDL_Surface* handle = SDL_ConvertSurfaceFormatAndColorspace(m_handle, format, colorspace, propID);
    if (handle != nullptr)
    {
        return rad::Ref<Surface>(RAD_NEW Surface(handle));
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_ConvertSurfaceFormatAndColorspace failed: {}", SDL_GetError());
        return nullptr;
    }
}

bool Surface::FillRect(const SDL_Rect* rect, Uint32 color)
{
    if (SDL_FillSurfaceRect(m_handle, rect, color) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_FillSurfaceRect failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::FillRects(const SDL_Rect* rects, int count, Uint32 color)
{
    if (SDL_FillSurfaceRects(m_handle, rects, count, color) == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_FillSurfaceRects failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::Blit(Surface* src, const SDL_Rect* srcRect, Surface* dst, SDL_Rect* dstRect)
{
    int res = SDL_BlitSurface(src->GetHandle(), srcRect, dst->GetHandle(), dstRect);
    if (res == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_BlitSurface failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::BlitUnchecked(Surface* src, const SDL_Rect* srcRect, Surface* dst, const SDL_Rect* dstRect)
{
    int res = SDL_BlitSurfaceUnchecked(src->GetHandle(), srcRect, dst->GetHandle(), dstRect);
    if (res == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_BlitSurfaceUnchecked failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::BlitScaledSoft(Surface* src, const SDL_Rect* srcRect,
    Surface* dst, const SDL_Rect* dstRect, SDL_ScaleMode scaleMode)
{
    int res = SDL_SoftStretch(src->GetHandle(), srcRect, dst->GetHandle(), dstRect, scaleMode);
    if (res == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SoftStretch failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::BlitScaled(Surface* src, const SDL_Rect* srcRect,
    Surface* dst, SDL_Rect* dstRect, SDL_ScaleMode scaleMode)
{
    int res = SDL_BlitSurfaceScaled(src->GetHandle(), srcRect, dst->GetHandle(), dstRect, scaleMode);
    if (res == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_BlitSurfaceScaled failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::BlitUncheckedScaled(Surface* src, const SDL_Rect* srcRect,
    Surface* dst, SDL_Rect* dstRect, SDL_ScaleMode scaleMode)
{
    int res = SDL_BlitSurfaceUncheckedScaled(src->GetHandle(), srcRect, dst->GetHandle(), dstRect, scaleMode);
    if (res == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_BlitSurfaceUncheckedScaled failed: {}", SDL_GetError());
        return false;
    }
}

bool Surface::ReadPixel(int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a)
{
    int res = SDL_ReadSurfacePixel(m_handle, x, y, r, g, b, a);
    if (res == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_ReadSurfacePixel failed: {}", SDL_GetError());
        return false;
    }
}

} // namespace rad
