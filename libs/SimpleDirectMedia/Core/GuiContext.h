#pragma once

#include "rad/Core/Common.h"
#include "Window.h"
#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer.h"

namespace sdl
{

class GuiContext : public rad::RefCounted<GuiContext>
{
public:
    GuiContext(rad::Ref<Window> window, rad::Ref<Renderer> renderer);
    ~GuiContext();

    ImGuiIO& GetIO() { return ImGui::GetIO(); }

    bool LoadFont(std::string_view fontFile, float fontSize,
        const ImFontConfig* font_cfg = nullptr, const ImWchar* glyph_ranges = nullptr);

    bool ProcessEvent(const SDL_Event& event);
    void StartFrame();
    void Render();
    void RenderDrawData();

private:
    rad::Ref<Window> m_window;
    rad::Ref<Renderer> m_renderer;

}; // class GuiContext

} // namespace sdl
