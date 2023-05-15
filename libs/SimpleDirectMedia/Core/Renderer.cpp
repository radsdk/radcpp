#include "Renderer.h"
#include "Window.h"
#include "Texture.h"
#include "rad/IO/Logging.h"

namespace sdl
{

Renderer::Renderer(Window* window) :
    m_window(window)
{
}

Renderer::~Renderer()
{
    if (m_renderer)
    {
        Destroy();
    }
}

bool Renderer::Init(int index, Uint32 flags)
{
    m_renderer = SDL_CreateRenderer(m_window->GetHandle(), index, flags);
    if (m_renderer)
    {
        SDL_GetRendererInfo(m_renderer, &m_rendererInfo);
        LogGlobal(Info, "SDL_Renderer created successfully: %s", m_rendererInfo.name);
        return true;
    }
    else
    {
        LogGlobal(Error, "SDL_CreateRenderer failed: %s", SDL_GetError());
        return false;
    }
}

void Renderer::Destroy()
{
    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
}

int Renderer::Clear()
{
    int res = SDL_RenderClear(m_renderer);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_RenderClear failed: %s", SDL_GetError());
    }
    return res;
}

void Renderer::Present()
{
    SDL_RenderPresent(m_renderer);
}

int Renderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int res = SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_SetRenderDrawColor failed: %s", SDL_GetError());
    }
    return res;
}

int Renderer::RenderGeometry(SDL_Texture* texture, rad::Span<SDL_Vertex> vertices, rad::Span<int> indices)
{
    int res = SDL_RenderGeometry(m_renderer, texture,
        vertices.data(), (int)vertices.size(),
        indices.data(), (int)indices.size());
    if (res != 0)
    {
        LogGlobal(Error, "SDL_RenderGeometry failed: %s", SDL_GetError());
    }
    return res;
}

int Renderer::CopyTexture(Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect)
{
    int res = SDL_RenderCopy(m_renderer, texture->GetHandle(),
        srcRect, dstRect);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_RenderCopy failed: %s", SDL_GetError());
    }
    return res;
}

int Renderer::CopyTextureEx(Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect,
    const double angle, const SDL_Point* center, const SDL_RendererFlip flip)
{
    int res = SDL_RenderCopyEx(m_renderer, texture->GetHandle(), srcRect, dstRect,
        angle, center, flip);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_RenderCopyEx failed: %s", SDL_GetError());
    }
    return res;
}

int Renderer::SetScale(float scaleX, float scaleY)
{
    int res = SDL_RenderSetScale(m_renderer, scaleX, scaleY);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_RenderSetScale failed: %s", SDL_GetError());
    }
    return res;
}

} // namespace sdl
