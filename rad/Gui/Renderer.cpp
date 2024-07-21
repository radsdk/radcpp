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

const char* Renderer::GetError()
{
    return SDL_GetError();
}

bool Renderer::GetOutputSize(int* w, int* h)
{
    int result = SDL_GetRenderOutputSize(m_handle, w, h);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetRenderOutputSize failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::GetCurrentOutputSize(int* w, int* h)
{
    int result = SDL_GetCurrentRenderOutputSize(m_handle, w, h);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetCurrentRenderOutputSize failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::SetRenderTarget(Texture* texture)
{
    int result = SDL_SetRenderTarget(m_handle, texture ? texture->GetHandle() : nullptr);
    if (result == 0)
    {
        m_renderTarget = texture;
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetRenderTarget failed: {}", SDL_GetError());
        return false;
    }
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

bool Renderer::SetLogicalPresentation(int w, int h,
    SDL_RendererLogicalPresentation mode, SDL_ScaleMode scaleMode)
{
    int result = SDL_SetRenderLogicalPresentation(m_handle, w, h, mode, scaleMode);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetRenderLogicalPresentation failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::GetLogicalPresentation(
    int* w, int* h, SDL_RendererLogicalPresentation* mode, SDL_ScaleMode* scaleMode)
{
    int result = SDL_GetRenderLogicalPresentation(m_handle, w, h, mode, scaleMode);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetRenderLogicalPresentation failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::TransformWindowCoordToRender(float windowX, float windowY, float* x, float* y)
{
    int result = SDL_RenderCoordinatesFromWindow(m_handle, windowX, windowY, x, y);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderCoordinatesFromWindow failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::TransformRenderCoordToWindow(float x, float y, float* windowX, float* windowY)
{
    int result = SDL_RenderCoordinatesToWindow(m_handle, x, y, windowX, windowY);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderCoordinatesToWindow failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::TransformCoordToRender(SDL_Event& event)
{
    int result = SDL_ConvertEventToRenderCoordinates(m_handle, &event);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_ConvertEventToRenderCoordinates failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::SetViewport(const SDL_Rect* rect)
{
    int result = SDL_SetRenderViewport(m_handle, rect);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetRenderViewport failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::SetViewportFull()
{
    return SetViewport(nullptr);
}

bool Renderer::GetViewport(SDL_Rect* rect)
{
    int result = SDL_GetRenderViewport(m_handle, rect);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetRenderViewport failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::HasViewportSet()
{
    return (SDL_RenderViewportSet(m_handle) == SDL_TRUE);
}

bool Renderer::SetClipRect(const SDL_Rect* rect)
{
    int result = SDL_SetRenderClipRect(m_handle, rect);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetRenderClipRect failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::DisableClipping()
{
    return SetClipRect(nullptr);
}

bool Renderer::GetClipRect(SDL_Rect* rect)
{
    int result = SDL_GetRenderClipRect(m_handle, rect);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetRenderClipRect failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::IsClipEnabled()
{
    return (SDL_RenderClipEnabled(m_handle) == SDL_TRUE);
}

bool Renderer::SetRenderScale(float scaleX, float scaleY)
{
    int result = SDL_SetRenderScale(m_handle, scaleX, scaleY);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetRenderScale failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::GetRenderScale(float* scaleX, float* scaleY)
{
    int result = SDL_GetRenderScale(m_handle, scaleX, scaleY);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetRenderScale failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::SetRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result = SDL_SetRenderDrawColor(m_handle, r, g, b, a);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetRenderDrawColor failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::SetRenderDrawColor(float r, float g, float b, float a)
{
    int result = SDL_SetRenderDrawColorFloat(m_handle, r, g, b, a);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetRenderDrawColorFloat failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::GetRenderDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a)
{
    int result = SDL_GetRenderDrawColor(m_handle, r, g, b, a);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetRenderDrawColor failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::GetRenderDrawColor(float* r, float* g, float* b, float* a)
{
    int result = SDL_GetRenderDrawColorFloat(m_handle, r, g, b, a);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetRenderDrawColorFloat failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::SetColorScale(float scale)
{
    int result = SDL_SetRenderColorScale(m_handle, scale);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetRenderColorScale failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::GetColorScale(float* scale)
{
    int result = SDL_GetRenderColorScale(m_handle, scale);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetRenderColorScale failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::SetBlendMode(SDL_BlendMode blendMode)
{
    int result = SDL_SetRenderDrawBlendMode(m_handle, blendMode);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetRenderDrawBlendMode failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::GetBlendMode(SDL_BlendMode* blendMode)
{
    int result = SDL_GetRenderDrawBlendMode(m_handle, blendMode);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetRenderDrawBlendMode failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::Clear()
{
    int result = SDL_RenderClear(m_handle);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderClear failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::DrawPoint(float x, float y)
{
    int result = SDL_RenderPoint(m_handle, x, y);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderPoint failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::DrawPoints(const SDL_FPoint* points, int count)
{
    int result = SDL_RenderPoints(m_handle, points, count);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderPoints failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::DrawLine(float x1, float y1, float x2, float y2)
{
    int result = SDL_RenderLine(m_handle, x1, y1, x2, y2);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderLine failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::DrawLines(const SDL_FPoint* points, int count)
{
    int result = SDL_RenderLines(m_handle, points, count);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderLines failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::DrawRect(const SDL_FRect* rect)
{
    int result = SDL_RenderRect(m_handle, rect);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderRect failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::DrawRects(const SDL_FRect* rect, int count)
{
    int result = SDL_RenderRects(m_handle, rect, count);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderRects failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::FillRect(const SDL_FRect* rect)
{
    int result = SDL_RenderFillRect(m_handle, rect);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderFillRect failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::FillRects(const SDL_FRect* rect, int count)
{
    int result = SDL_RenderFillRects(m_handle, rect, count);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderFillRects failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::DrawTexture(Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect)
{
    int result = SDL_RenderTexture(m_handle, texture->GetHandle(), srcRect, dstRect);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderTexture failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::DrawTextureRotated(Texture* texture,
    const SDL_FRect* srcRect, const SDL_FRect* dstRect,
    const double angle, const SDL_FPoint* center,
    const SDL_FlipMode flip)
{
    int result = SDL_RenderTextureRotated(m_handle, texture->GetHandle(),
        srcRect, dstRect, angle, center, flip);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderTextureRotated failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::RenderGeometry(Texture* texture,
    const SDL_Vertex* vertices, int numVertices,
    const int* indices, int numIndices)
{
    int result = SDL_RenderGeometry(m_handle, texture->GetHandle(),
        vertices, numVertices, indices, numIndices);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderGeometry failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::RenderGeometryRaw(Texture* texture,
    const float* xy, int xyStride,
    const SDL_FColor* color, int colorStride,
    const float* uv, int uvStride,
    int numVertices,
    const void* indices, int numIndices, int indexType)
{
    int result = SDL_RenderGeometryRaw(m_handle, texture->GetHandle(),
        xy, xyStride, color, colorStride, uv, uvStride, numVertices, indices, numIndices, indexType);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderGeometryRaw failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::Present()
{
    int result = SDL_RenderPresent(m_handle);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_RenderPresent failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::Flush()
{
    int result = SDL_FlushRenderer(m_handle);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_FlushRenderer failed: {}", SDL_GetError());
        return false;
    }
}

bool Renderer::SetVSync(int vsync)
{
    int result = SDL_SetRenderVSync(m_handle, vsync);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_SetRenderVSync({}) failed: {}",
            vsync, SDL_GetError());
        return false;
    }
}

bool Renderer::GetVSync(int* vsync)
{
    int result = SDL_GetRenderVSync(m_handle, vsync);
    if (result == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "SDL_GetRenderVSync failed: {}", SDL_GetError());
        return false;
    }
}

} // namespace rad
