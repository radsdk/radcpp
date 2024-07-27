#pragma once

#include <rad/Gui/Window.h>
#include <rad/Gui/Renderer.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

namespace rad
{

class GuiContext : public RefCounted<GuiContext>
{
public:
    GuiContext(Window* window, Renderer* renderer);
    ~GuiContext();

    ImGuiContext* GetContext() { return m_context; }

    bool Init();
    void Destroy();

    ImGuiIO& GetIO() const { return ImGui::GetIO(); }

    void SetClearColor(const ImColor& color) { m_clearColor = color; }

    bool ProcessEvent(const SDL_Event& event);
    void NewFrame();
    void Render();

private:
    Window* m_window;
    Renderer* m_renderer;
    ImGuiContext* m_context = nullptr;
    ImColor m_clearColor = {};

}; // class GuiContext

} // namespace rad
