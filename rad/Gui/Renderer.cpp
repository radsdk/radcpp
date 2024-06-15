#include <rad/Gui/Renderer.h>

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
    auto logger = GetGuiLogger();
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
        RAD_LOG(logger, info, "Renderer created: {}", m_name);
        m_propID = SDL_GetRendererProperties(m_handle);
        if (m_propID == 0)
        {
            RAD_LOG(logger, err, "SDL_GetRendererProperties failed: {}", SDL_GetError());
        }
        return true;
    }
    else
    {
        RAD_LOG(logger, err, "SDL_CreateRenderer failed: {}", SDL_GetError());
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
