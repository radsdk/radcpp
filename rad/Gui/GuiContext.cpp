#include <rad/Gui/GuiContext.h>

namespace rad
{

GuiContext::GuiContext(Window* window, Renderer* renderer) :
    m_window(window),
    m_renderer(renderer)
{
}

GuiContext::~GuiContext()
{
    Destroy();
}

bool GuiContext::Init()
{
    IMGUI_CHECKVERSION();
    m_context = ImGui::CreateContext();
    if (m_context)
    {
        RAD_LOG(GetGuiLogger(), info, "ImGuiContext created: {} ({})",
            IMGUI_VERSION, IMGUI_VERSION_NUM);
    }
    else
    {
        RAD_LOG(GetGuiLogger(), err, "ImGui::CreateContext failed!");
        return false;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    ImGui_ImplSDL3_InitForSDLRenderer(
        m_window->GetHandle(), m_renderer->GetHandle());
    ImGui_ImplSDLRenderer3_Init(m_renderer->GetHandle());

    return true;
}

void GuiContext::Destroy()
{
    if (m_context)
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        m_context = nullptr;
    }
}

bool GuiContext::ProcessEvent(const SDL_Event& event)
{
    return ImGui_ImplSDL3_ProcessEvent(&event);
}

void GuiContext::NewFrame()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void GuiContext::Render()
{
    ImGui::Render();
    m_renderer->SetRenderDrawColor(
        Uint8(m_clearColor.Value.x * 255),
        Uint8(m_clearColor.Value.y * 255),
        Uint8(m_clearColor.Value.z * 255),
        Uint8(m_clearColor.Value.w * 255));
    m_renderer->Clear();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer->GetHandle());
    m_renderer->Present();
}

} // namespace rad
