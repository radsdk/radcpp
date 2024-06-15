#pragma once

#include <rad/Gui/Window.h>
#include <SDL3/SDL_render.h>

namespace rad
{

class Renderer : public rad::RefCounted<Renderer>
{
public:
    Renderer(Window* window);
    ~Renderer();

    bool Init();
    void Destroy();

    int GetOutputSize(int* w, int* h);
    int GetCurrentOutputSize(int* w, int* h);

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

    int Present();
    int Flush();

    int SetVSync(int vsync);
    int GetVSync(int* vsync);

private:
    Window* m_window;
    SDL_Renderer* m_handle = nullptr;
    const char* m_name = nullptr;
    SDL_PropertiesID m_propID = 0;
}; // class Renderer

} // namespace rad
