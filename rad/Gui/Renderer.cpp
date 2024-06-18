#include <rad/Gui/Renderer.h>
#include <rad/Gui/Texture.h>

namespace rad
{

Renderer::Renderer(Window* window) :
    m_window(window)
{
}

Renderer::~Renderer()
{
    Destroy();
}

bool Renderer::Init()
{
    const char* driver = nullptr;

#if defined(RAD_OS_WINDOWS)
    for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i)
    {
        const char* name = SDL_GetRenderDriver(i);
        if (rad::StrEqual(name, "direct3d12") ||
            rad::StrEqual(name, "direct3d11"))
        {
            driver = name;
            break;
        }
    }
#endif
    m_handle = SDL_CreateRenderer(m_window->GetHandle(), driver);
    if (m_handle)
    {
        m_name = SDL_GetRendererName(m_handle);
        RAD_LOG(GetGuiLogger(), info, "Renderer created: {}", m_name);
        m_propID = SDL_GetRendererProperties(m_handle);
        if (m_propID == 0)
        {
            RAD_LOG(GetGuiLogger(), err, "SDL_GetRendererProperties failed: {}", SDL_GetError());
        }
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_CreateRenderer failed: {}", SDL_GetError());
        return false;
    }
}

void Renderer::Destroy()
{
    if (m_handle)
    {
        SDL_DestroyRenderer(m_handle);
        m_handle = nullptr;
    }
}

int Renderer::GetOutputSize(int* w, int* h)
{
    return SDL_GetRenderOutputSize(m_handle, w, h);
}

int Renderer::GetCurrentOutputSize(int* w, int* h)
{
    return SDL_GetCurrentRenderOutputSize(m_handle, w, h);
}

int Renderer::SetRenderTarget(Texture* texture)
{
    return SDL_SetRenderTarget(m_handle, texture ? texture->GetHandle() : nullptr);
}

Texture* Renderer::GetRenderTarget()
{
    if (m_renderTarget)
    {
#if defined(_DEBUG)
        SDL_Texture* handle = SDL_GetRenderTarget(m_handle);
        assert(m_renderTarget->GetHandle() == handle);
#endif
        return m_renderTarget;
    }
    else
    {
        return nullptr;
    }
}

int Renderer::SetLogicalPresentation(int w, int h,
    SDL_RendererLogicalPresentation mode, SDL_ScaleMode scaleMode)
{
    return SDL_SetRenderLogicalPresentation(m_handle, w, h, mode, scaleMode);
}

int Renderer::GetLogicalPresentation(int* w, int* h, SDL_RendererLogicalPresentation* mode, SDL_ScaleMode* scaleMode)
{
    return SDL_GetRenderLogicalPresentation(m_handle, w, h, mode, scaleMode);
}

int Renderer::TransformWindowToRender(float windowX, float windowY, float* x, float* y)
{
    return SDL_RenderCoordinatesFromWindow(m_handle, windowX, windowY, x, y);
}

int Renderer::TransformRenderToWindow(float x, float y, float* windowX, float* windowY)
{
    return SDL_RenderCoordinatesToWindow(m_handle, x, y, windowX, windowY);
}

int Renderer::TransformEvent(SDL_Event& event)
{
    return SDL_ConvertEventToRenderCoordinates(m_handle, &event);
}

int Renderer::SetViewport(const SDL_Rect& rect)
{
    return SDL_SetRenderViewport(m_handle, &rect);
}

int Renderer::GetViewport(SDL_Rect& rect)
{
    return SDL_GetRenderViewport(m_handle, &rect);
}

bool Renderer::HasViewportSet()
{
    return SDL_RenderViewportSet(m_handle) == SDL_TRUE;
}

int Renderer::SetClipRect(const SDL_Rect& rect)
{
    return SDL_SetRenderClipRect(m_handle, &rect);
}

int Renderer::GetClipRect(SDL_Rect& rect)
{
    return SDL_GetRenderClipRect(m_handle, &rect);
}

bool Renderer::IsClipEnabled()
{
    return SDL_RenderClipEnabled(m_handle) == SDL_TRUE;
}

int Renderer::SetRenderScale(float scaleX, float scaleY)
{
    return SDL_SetRenderScale(m_handle, scaleX, scaleY);
}

int Renderer::GetRenderScale(float* scaleX, float* scaleY)
{
    return SDL_GetRenderScale(m_handle, scaleX, scaleY);
}

int Renderer::SetRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return SDL_SetRenderDrawColor(m_handle, r, g, b, a);
}

int Renderer::SetRenderDrawColor(float r, float g, float b, float a)
{
    return SDL_SetRenderDrawColorFloat(m_handle, r, g, b, a);
}

int Renderer::GetRenderDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a)
{
    return SDL_GetRenderDrawColor(m_handle, r, g, b, a);
}

int Renderer::GetRenderDrawColor(float* r, float* g, float* b, float* a)
{
    return SDL_GetRenderDrawColorFloat(m_handle, r, g, b, a);
}

int Renderer::SetColorScale(float scale)
{
    return SDL_SetRenderColorScale(m_handle, scale);
}

int Renderer::GetColorScale(float* scale)
{
    return SDL_GetRenderColorScale(m_handle, scale);
}

int Renderer::SetBlendMode(SDL_BlendMode blendMode)
{
    return SDL_SetRenderDrawBlendMode(m_handle, blendMode);
}

int Renderer::GetBlendMode(SDL_BlendMode* blendMode)
{
    return SDL_GetRenderDrawBlendMode(m_handle, blendMode);
}

int Renderer::Clear()
{
    return SDL_RenderClear(m_handle);
}

int Renderer::DrawPoint(float x, float y)
{
    return SDL_RenderPoint(m_handle, x, y);
}

int Renderer::DrawPoints(const SDL_FPoint* points, int count)
{
    return SDL_RenderPoints(m_handle, points, count);
}

int Renderer::DrawLine(float x1, float y1, float x2, float y2)
{
    return SDL_RenderLine(m_handle, x1, y1, x2, y2);
}

int Renderer::DrawLines(const SDL_FPoint* points, int count)
{
    return SDL_RenderLines(m_handle, points, count);
}

int Renderer::DrawRect(const SDL_FRect* rect)
{
    return SDL_RenderRect(m_handle, rect);
}

int Renderer::DrawRects(const SDL_FRect* rect, int count)
{
    return SDL_RenderRects(m_handle, rect, count);
}

int Renderer::FillRect(const SDL_FRect* rect)
{
    return SDL_RenderFillRect(m_handle, rect);
}

int Renderer::FillRects(const SDL_FRect* rect, int count)
{
    return SDL_RenderFillRects(m_handle, rect, count);
}

int Renderer::DrawTexture(Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect)
{
    return SDL_RenderTexture(m_handle, texture->GetHandle(), srcRect, dstRect);
}

int Renderer::DrawTextureRotated(Texture* texture,
    const SDL_FRect* srcRect, const SDL_FRect* dstRect,
    const double angle, const SDL_FPoint* center,
    const SDL_FlipMode flip)
{
    return SDL_RenderTextureRotated(m_handle, texture->GetHandle(),
        srcRect, dstRect, angle, center, flip);
}

int Renderer::RenderGeometry(Texture* texture,
    const SDL_Vertex* vertices, int numVertices,
    const int* indices, int numIndices)
{
    return SDL_RenderGeometry(m_handle, texture->GetHandle(),
        vertices, numVertices, indices, numIndices);
}

int Renderer::RenderGeometryRaw(Texture* texture,
    const float* xy, int xyStride,
    const SDL_Color* color, int colorStride,
    const float* uv, int uvStride,
    int numVertices,
    const void* indices, int numIndices, int indexType)
{
    return SDL_RenderGeometryRaw(m_handle, texture->GetHandle(),
        xy, xyStride, color, colorStride, uv, uvStride, numVertices, indices, numIndices, indexType);
}

int Renderer::RenderGeometryRaw(Texture* texture,
    const float* xy, int xyStride,
    const SDL_FColor* color, int colorStride,
    const float* uv, int uvStride,
    int numVertices,
    const void* indices, int numIndices, int indexType)
{
    return SDL_RenderGeometryRawFloat(m_handle, texture->GetHandle(),
        xy, xyStride, color, colorStride, uv, uvStride, numVertices, indices, numIndices, indexType);
}

int Renderer::Present()
{
    return SDL_RenderPresent(m_handle);
}

int Renderer::Flush()
{
    return SDL_FlushRenderer(m_handle);
}

int Renderer::SetVSync(int vsync)
{
    return SDL_SetRenderVSync(m_handle, vsync);
}

int Renderer::GetVSync(int* vsync)
{
    return SDL_GetRenderVSync(m_handle, vsync);
}

} // namespace rad
