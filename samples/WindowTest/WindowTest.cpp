#include "WindowTest.h"
#include "rad/IO/Logging.h"

WindowTest::WindowTest()
{
    LogGlobal(Info, "WindowTest::WindowTest()");
}

WindowTest::~WindowTest()
{
    LogGlobal(Info, "WindowTest::~WindowTest()");
}

bool WindowTest::Init()
{
    float windowScale = sdl::Application::GetInstance()->GetDisplayDPI(0) / 96.0f;
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    Create("WindowTest",
        SDL_WINDOWPOS_CENTERED_DISPLAY(0), SDL_WINDOWPOS_CENTERED_DISPLAY(0),
        int(800 * windowScale), int(600 * windowScale),
        windowFlags);

    m_renderer = new sdl::Renderer(this);
    m_renderer->Init();

    m_gui = new sdl::GuiContext(this, m_renderer);

    // Load custom fonts
    float fontSize = GetDisplayDPI() / 72.0f * 12.0f;
#ifdef _WIN32
    // https://github.com/ocornut/imgui/blob/master/docs/FONTS.md
    auto fonts = m_gui->GetFonts();
    fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", fontSize);
    fonts->Build();
#endif

    return true;
}

bool WindowTest::OnEvent(const SDL_Event& event)
{
    if (m_gui)
    {
        m_gui->ProcessEvent(event);
    }
    return Window::OnEvent(event);
}

void WindowTest::OnShown()
{
    LogGlobal(Info, "OnShown");
}

void WindowTest::OnHidden()
{
    LogGlobal(Info, "OnHidden");
}

void WindowTest::OnExposed()
{
    LogGlobal(Info, "OnExposed");
}

void WindowTest::OnMoved(int x, int y)
{
    LogGlobal(Info, "OnMoved: %4d, %4d", x, y);
}

void WindowTest::OnResized(int width, int height)
{
    LogGlobal(Info, "OnResized: %4d, %4d", width, height);
}

void WindowTest::OnMinimized()
{
    LogGlobal(Info, "OnMinimized");
}

void WindowTest::OnMaximized()
{
    LogGlobal(Info, "OnMaximized");
}

void WindowTest::OnRestored()
{
    LogGlobal(Info, "OnRestored");
}

void WindowTest::OnEnter()
{
    LogGlobal(Info, "OnEnter");
}

void WindowTest::OnLeave()
{
    LogGlobal(Info, "OnLeave");
}

void WindowTest::OnKeyDown(const SDL_KeyboardEvent& keyDown)
{
    LogGlobal(Info, "OnKeyDown: %s", SDL_GetKeyName(keyDown.keysym.sym));
}

void WindowTest::OnKeyUp(const SDL_KeyboardEvent& keyUp)
{
    LogGlobal(Info, "OnKeyUp: %s", SDL_GetKeyName(keyUp.keysym.sym));
}

void WindowTest::OnMouseMove(const SDL_MouseMotionEvent& mouseMotion)
{
    m_mouseMoveStr = rad::StrPrint("OnMouseMove: x = %6d (%+6d); y = %6d (%+6d);",
        mouseMotion.x, mouseMotion.xrel,
        mouseMotion.y, mouseMotion.yrel
    );
}

void WindowTest::OnMouseButtonDown(const SDL_MouseButtonEvent& mouseButton)
{
    LogGlobal(Info, "OnMouseButtonDown: %s", GetMouseButtonName(mouseButton.button));
}

void WindowTest::OnMouseButtonUp(const SDL_MouseButtonEvent& mouseButton)
{
    LogGlobal(Info, "OnMouseButtonUp: %s", GetMouseButtonName(mouseButton.button));
}

void WindowTest::OnMouseWheel(const SDL_MouseWheelEvent& mouseWheel)
{
    LogGlobal(Info, "OnMouseWheel: %+d", mouseWheel.y);
}

void WindowTest::OnRender()
{
    m_gui->StartFrame();

    ImDrawList* fgDrawList = ImGui::GetForegroundDrawList();
    fgDrawList->AddText(ImVec2(0, 0), ImColor(1.0f, 1.0f, 1.0f, 1.0f),
        m_mouseMoveStr.c_str());

    m_gui->Render();

    m_renderer->SetScale(
        m_gui->GetIO().DisplayFramebufferScale.x,
        m_gui->GetIO().DisplayFramebufferScale.y
    );
    m_renderer->SetDrawColor(
        Uint8(m_clearColor.x * 255),
        Uint8(m_clearColor.y * 255),
        Uint8(m_clearColor.z * 255),
        Uint8(m_clearColor.w * 255)
    );
    m_renderer->Clear();
    m_gui->RenderDrawData();
    m_renderer->Present();
}

const char* WindowTest::GetMouseButtonName(Uint8 button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT: return "SDL_BUTTON_LEFT";
    case SDL_BUTTON_MIDDLE: return "SDL_BUTTON_MIDDLE";
    case SDL_BUTTON_RIGHT: return "SDL_BUTTON_RIGHT";
    case SDL_BUTTON_X1: return "SDL_BUTTON_X1";
    case SDL_BUTTON_X2: return "SDL_BUTTON_X2";
    }
    return "SDL_BUTTON_UNKNOWN";
}
