#pragma once

#include <rad/Gui/Window.h>
#include <SDL3/SDL_render.h>

namespace rad
{

class Texture;

class Renderer : public rad::RefCounted<Renderer>
{
public:
    Renderer(Window* window);
    ~Renderer();

    bool Init();
    void Destroy();

    SDL_Renderer* GetHandle() { return m_handle; }
    const char* GetError();
    const char* GetName() const { return m_name; }

    bool GetOutputSize(int* w, int* h);
    bool GetCurrentOutputSize(int* w, int* h);

    bool SetRenderTarget(Texture* texture);
    bool SetRenderToWindow() { return SetRenderTarget(nullptr); }
    Texture* GetRenderTarget();

    bool SetLogicalPresentation(int w, int h,
        SDL_RendererLogicalPresentation mode, SDL_ScaleMode scaleMode);
    bool GetLogicalPresentation(int* w, int* h,
        SDL_RendererLogicalPresentation* mode, SDL_ScaleMode* scaleMode);

    bool TransformWindowCoordToRender(float windowX, float windowY, float* x, float* y);
    bool TransformRenderCoordToWindow(float x, float y, float* windowX, float* windowY);
    bool TransformCoordToRender(SDL_Event& event);

    bool SetViewport(const SDL_Rect* rect);
    bool SetViewportFull();
    bool GetViewport(SDL_Rect* rect);
    bool SetViewport(const SDL_Rect& rect) { return SetViewport(&rect); }
    bool GetViewport(SDL_Rect& rect) { return GetViewport(&rect); }
    bool HasViewportSet();
    bool SetClipRect(const SDL_Rect* rect);
    bool DisableClipping();
    bool GetClipRect(SDL_Rect* rect);
    bool SetClipRect(const SDL_Rect& rect) { return SetClipRect(&rect); }
    bool GetClipRect(SDL_Rect& rect) { return GetClipRect(&rect); }
    bool IsClipEnabled();
    bool SetRenderScale(float scaleX, float scaleY);
    bool GetRenderScale(float* scaleX, float* scaleY);

    bool SetRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    bool SetRenderDrawColor(float r, float g, float b, float a);
    bool GetRenderDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a);
    bool GetRenderDrawColor(float* r, float* g, float* b, float* a);

    bool SetColorScale(float scale);
    bool GetColorScale(float* scale);
    bool SetBlendMode(SDL_BlendMode blendMode);
    bool GetBlendMode(SDL_BlendMode* blendMode);

    bool Clear();

    bool DrawPoint(float x, float y);
    bool DrawPoints(const SDL_FPoint* points, int count);
    bool DrawLine(float x1, float y1, float x2, float y2);
    bool DrawLines(const SDL_FPoint* points, int count);
    bool DrawRect(const SDL_FRect* rect);
    bool DrawRects(const SDL_FRect* rect, int count);
    bool FillRect(const SDL_FRect* rect);
    bool FillRects(const SDL_FRect* rect, int count);

    bool DrawTexture(Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect);
    bool DrawTextureRotated(Texture* texture,
        const SDL_FRect* srcRect, const SDL_FRect* dstRect,
        const double angle, const SDL_FPoint* center,
        const SDL_FlipMode flip);
    bool RenderGeometry(Texture* texture,
        const SDL_Vertex* vertices, int numVertices,
        const int* indices, int numIndices);
    bool RenderGeometryRaw(Texture* texture,
        const float* xy, int xyStride,
        const SDL_FColor* color, int colorStride,
        const float* uv, int uvStride,
        int numVertices,
        const void* indices, int numIndices, int indexType);

    bool Present();
    bool Flush();

    bool SetVSync(int vsync);
    bool GetVSync(int* vsync);

private:
    Window* m_window;
    SDL_Renderer* m_handle = nullptr;
    const char* m_name = nullptr;
    SDL_PropertiesID m_propID = 0;

    Texture* m_renderTarget = nullptr;

}; // class Renderer

} // namespace rad
