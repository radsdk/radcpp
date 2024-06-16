#include <rad/Gui/Surface.h>
#include <rad/Gui/Window.h>

namespace rad
{

rad::Ref<Surface> Surface::Create(int width, int height, SDL_PixelFormatEnum format)
{
    auto logger = GetGuiLogger();
    SDL_Surface* handle = SDL_CreateSurface(width, height, format);
    if (handle)
    {
        return new Surface(handle);
    }
    else
    {
        RAD_LOG(logger, err, "SDL_CreateSurface failed: {}", SDL_GetError());
        return nullptr;
    }
}

rad::Ref<Surface> Surface::CreateFormPixels(void* pixels, int width, int height, int pitch, SDL_PixelFormatEnum format)
{
    auto logger = GetGuiLogger();
    SDL_Surface* handle = SDL_CreateSurfaceFrom(pixels, width, height, pitch, format);
    if (handle)
    {
        return new Surface(handle);
    }
    else
    {
        RAD_LOG(logger, err, "SDL_CreateSurfaceFrom failed: {}", SDL_GetError());
        return nullptr;
    }
}

rad::Ref<Surface> Surface::CreateFromBMP(SDL_IOStream* src, SDL_bool closeio)
{
    auto logger = GetGuiLogger();
    SDL_Surface* handle = SDL_LoadBMP_IO(src, closeio);
    if (handle)
    {
        return new Surface(handle);
    }
    else
    {
        RAD_LOG(logger, err, "SDL_LoadBMP_IO failed: {}", SDL_GetError());
        return nullptr;
    }
}

rad::Ref<Surface> Surface::CreateFromBMP(const char* file)
{
    auto logger = GetGuiLogger();
    SDL_Surface* handle = SDL_LoadBMP(file);
    if (handle)
    {
        return new Surface(handle);
    }
    else
    {
        RAD_LOG(logger, err, "SDL_LoadBMP_IO failed: {}", SDL_GetError());
        return nullptr;
    }
}

Surface::Surface(SDL_Surface* handle) :
    m_handle(handle)
{
    auto logger = GetGuiLogger();
    m_propID = SDL_GetSurfaceProperties(m_handle);
    if (m_propID == 0)
    {
        RAD_LOG(logger, err, "SDL_GetSurfaceProperties failed: {}", SDL_GetError());
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

int Surface::SaveBMP(SDL_IOStream* dst, SDL_bool closeio)
{
    return SDL_SaveBMP_IO(m_handle, dst, closeio);
}

int Surface::SaveBMP(const char* file)
{
    return SDL_SaveBMP(m_handle, file);
}

int Surface::SetColorSpace(SDL_Colorspace colorspace)
{
    return SDL_SetSurfaceColorspace(m_handle, colorspace);
}

int Surface::GetColorSpace(SDL_Colorspace* colorspace)
{
    return SDL_GetSurfaceColorspace(m_handle, colorspace);
}

int Surface::SetPalette(SDL_Palette* palette)
{
    return SDL_SetSurfacePalette(m_handle, palette);
}

int Surface::Lock()
{
    return SDL_LockSurface(m_handle);
}

void Surface::Unlock()
{
    SDL_UnlockSurface(m_handle);
}

int Surface::SetRLE(int flag)
{
    return SDL_SetSurfaceRLE(m_handle, flag);
}

bool Surface::HasRLE()
{
    return SDL_SurfaceHasRLE(m_handle) == SDL_TRUE;
}

int Surface::SetColorKey(int flag, Uint32 key)
{
    return SDL_SetSurfaceColorKey(m_handle, flag, key);
}

bool Surface::HasColorKey()
{
    return SDL_SurfaceHasColorKey(m_handle) == SDL_TRUE;
}

int Surface::GetColorKey(Uint32* key)
{
    return SDL_GetSurfaceColorKey(m_handle, key);
}

int Surface::SetColorMod(Uint8 r, Uint8 g, Uint8 b)
{
    return SDL_SetSurfaceColorMod(m_handle, r, g, b);
}

int Surface::GetColorMod(Uint8* r, Uint8* g, Uint8* b)
{
    return SDL_GetSurfaceColorMod(m_handle, r, g, b);
}

int Surface::SetAlphaMode(Uint8 alpha)
{
    return SDL_SetSurfaceAlphaMod(m_handle, alpha);
}

int Surface::GetAlphaMode(Uint8* alpha)
{
    return SDL_GetSurfaceAlphaMod(m_handle, alpha);
}

int Surface::SetBlendMode(SDL_BlendMode blendMode)
{
    return SDL_SetSurfaceBlendMode(m_handle, blendMode);
}

int Surface::GetBlendMode(SDL_BlendMode* blendMode)
{
    return SDL_GetSurfaceBlendMode(m_handle, blendMode);
}

int Surface::SetClipRect(const SDL_Rect* rect)
{
    return SDL_SetSurfaceClipRect(m_handle, rect);
}

int Surface::GetClipRect(SDL_Rect* rect)
{
    return SDL_GetSurfaceClipRect(m_handle, rect);
}

int Surface::Flip(SDL_FlipMode flip)
{
    return SDL_FlipSurface(m_handle, flip);
}

rad::Ref<Surface> Surface::Duplicate()
{
    SDL_Surface* handle = SDL_DuplicateSurface(m_handle);
    return rad::Ref<Surface>(new Surface(handle));
}

rad::Ref<Surface> Surface::Convert(const SDL_PixelFormat* format)
{
    SDL_Surface* handle = SDL_ConvertSurface(m_handle, format);
    return rad::Ref<Surface>(new Surface(handle));
}

rad::Ref<Surface> Surface::Convert(SDL_PixelFormatEnum format)
{
    SDL_Surface* handle = SDL_ConvertSurfaceFormat(m_handle, format);
    return rad::Ref<Surface>(new Surface(handle));
}

rad::Ref<Surface> Surface::Convert(SDL_PixelFormatEnum format, SDL_Colorspace colorspace,
    SDL_PropertiesID propID)
{
    SDL_Surface* handle = SDL_ConvertSurfaceFormatAndColorspace(m_handle, format, colorspace, propID);
    return rad::Ref<Surface>(new Surface(handle));
}

int Surface::FillRect(const SDL_Rect* rect, Uint32 color)
{
    return SDL_FillSurfaceRect(m_handle, rect, color);
}

int Surface::FillRects(const SDL_Rect* rects, int count, Uint32 color)
{
    return SDL_FillSurfaceRects(m_handle, rects, count, color);
}

int Surface::Blit(Surface* src, const SDL_Rect* srcRect, Surface* dst, SDL_Rect* dstRect)
{
    return SDL_BlitSurface(src->GetHandle(), srcRect, dst->GetHandle(), dstRect);
}

int Surface::BlitUnchecked(Surface* src, const SDL_Rect* srcRect, Surface* dst, const SDL_Rect* dstRect)
{
    return SDL_BlitSurfaceUnchecked(src->GetHandle(), srcRect, dst->GetHandle(), dstRect);
}

int Surface::BlitScaledSoft(Surface* src, const SDL_Rect* srcRect,
    Surface* dst, const SDL_Rect* dstRect, SDL_ScaleMode scaleMode)
{
    return SDL_SoftStretch(src->GetHandle(), srcRect, dst->GetHandle(), dstRect, scaleMode);
}

int Surface::BlitScaled(Surface* src, const SDL_Rect* srcRect,
    Surface* dst, SDL_Rect* dstRect, SDL_ScaleMode scaleMode)
{
    return SDL_BlitSurfaceScaled(src->GetHandle(), srcRect, dst->GetHandle(), dstRect, scaleMode);
}

int Surface::BlitUncheckedScaled(Surface* src, const SDL_Rect* srcRect,
    Surface* dst, SDL_Rect* dstRect, SDL_ScaleMode scaleMode)
{
    return SDL_BlitSurfaceUncheckedScaled(src->GetHandle(), srcRect, dst->GetHandle(), dstRect, scaleMode);
}

int Surface::ReadPixel(int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a)
{
    return SDL_ReadSurfacePixel(m_handle, x, y, r, g, b, a);
}

} // namespace rad
