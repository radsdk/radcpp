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

    int GetOutputSize(int* w, int* h);
    int GetCurrentOutputSize(int* w, int* h);

    int SetRenderTarget(Texture* texture);
    Texture* GetRenderTarget();

    int SetLogicalPresentation(int w, int h,
        SDL_RendererLogicalPresentation mode, SDL_ScaleMode scaleMode);
    int GetLogicalPresentation(int* w, int* h,
        SDL_RendererLogicalPresentation* mode, SDL_ScaleMode* scaleMode);

    int TransformWindowToRender(float windowX, float windowY, float* x, float* y);
    int TransformRenderToWindow(float x, float y, float* windowX, float* windowY);
    int TransformEvent(SDL_Event& event);

    int SetViewport(const SDL_Rect& rect);
    int GetViewport(SDL_Rect& rect);
    bool HasViewportSet();
    int SetClipRect(const SDL_Rect& rect);
    int GetClipRect(SDL_Rect& rect);
    bool IsClipEnabled();
    int SetRenderScale(float scaleX, float scaleY);
    int GetRenderScale(float* scaleX, float* scaleY);

    int SetRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    int SetRenderDrawColor(float r, float g, float b, float a);
    int GetRenderDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a);
    int GetRenderDrawColor(float* r, float* g, float* b, float* a);

    int SetColorScale(float scale);
    int GetColorScale(float* scale);
    int SetBlendMode(SDL_BlendMode blendMode);
    int GetBlendMode(SDL_BlendMode* blendMode);

    int Clear();

    int DrawPoint(float x, float y);
    int DrawPoints(const SDL_FPoint* points, int count);
    int DrawLine(float x1, float y1, float x2, float y2);
    int DrawLines(const SDL_FPoint* points, int count);
    int DrawRect(const SDL_FRect* rect);
    int DrawRects(const SDL_FRect* rect, int count);
    int FillRect(const SDL_FRect* rect);
    int FillRects(const SDL_FRect* rect, int count);

    int DrawTexture(Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect);
    int DrawTextureRotated(Texture* texture,
        const SDL_FRect* srcRect, const SDL_FRect* dstRect,
        const double angle, const SDL_FPoint* center,
        const SDL_FlipMode flip);
    int RenderGeometry(Texture* texture,
        const SDL_Vertex* vertices, int numVertices,
        const int* indices, int numIndices);
    int RenderGeometryRaw(Texture* texture,
        const float* xy, int xyStride,
        const SDL_Color* color, int colorStride,
        const float* uv, int uvStride,
        int numVertices,
        const void* indices, int numIndices, int indexType);
    int RenderGeometryRaw(Texture* texture,
        const float* xy, int xyStride,
        const SDL_FColor* color, int colorStride,
        const float* uv, int uvStride,
        int numVertices,
        const void* indices, int numIndices, int indexType
    );

    int Present();
    int Flush();

    int SetVSync(int vsync);
    int GetVSync(int* vsync);

private:
    Window* m_window;
    SDL_Renderer* m_handle = nullptr;
    const char* m_name = nullptr;
    SDL_PropertiesID m_propID = 0;

    Texture* m_renderTarget = nullptr;

}; // class Renderer

} // namespace rad
