#include "Renderer.h"
#include "Window.h"
#include "Texture.h"
#include "rad/Core/Flags.h"
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

std::vector<SDL_RendererInfo> Renderer::GetRendererInfos()
{
    std::vector<SDL_RendererInfo> infos;
    int infoCount = SDL_GetNumRenderDrivers();
    if (infoCount >= 0)
    {
        infos.resize(infoCount);
        for (int i = 0; i < infoCount; ++i)
        {
            if (SDL_GetRenderDriverInfo(i, &infos[i]) != 0)
            {
                LogGlobal(Error, "SDL_GetRenderDriverInfo failed: %s", SDL_GetError());
            }
        }
    }
    return infos;
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

bool Renderer::Init()
{
    std::vector<SDL_RendererInfo> rendererInfos = GetRendererInfos();
    int rendererIndex = -1;
    Uint32 flagsRequested = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
    for (size_t i = 0; i < rendererInfos.size(); ++i)
    {
        LogGlobal(Info, "Renderer#%llu: %s", i, rendererInfos[i].name);
    }
    const char* rendererNames[] =
    {
        "direct3d11",
        "direct3d12",
        "opengl",
    };
    for (size_t candidateIndex = 0; candidateIndex < std::size(rendererNames); ++candidateIndex)
    {
        for (size_t infoIndex = 0; infoIndex < rendererInfos.size(); ++infoIndex)
        {
            if (rad::StrCaseEqual(rendererInfos[infoIndex].name, rendererNames[candidateIndex]) &&
                rad::Flags32<SDL_RendererFlags>(rendererInfos[infoIndex].flags).HasBits(flagsRequested))
            {
                rendererIndex = int(infoIndex);
                break;
            }
        }
        if (rendererIndex != -1)
        {
            break;
        }
    }
    return Init(rendererIndex, flagsRequested);
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

int Renderer::SetLogicalSize(int w, int h)
{
    int res = SDL_RenderSetLogicalSize(m_renderer, w, h);
    if (res != 0)
    {
        LogGlobal(Error, "SDL_RenderSetLogicalSize failed: %s", SDL_GetError());
    }
    return res;
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
