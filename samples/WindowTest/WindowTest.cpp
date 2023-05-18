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
    std::vector<SDL_RendererInfo> rendererInfos = m_renderer->GetRendererInfos();
    int rendererIndex = -1;
    for (size_t i = 0; i < rendererInfos.size(); ++i)
    {
        LogGlobal(Info, "Renderer#%llu: %s", i, rendererInfos[i].name);
    }
    const char* rendererCandidates[] =
    {
        "direct3d11",
        "direct3d12",
        "opengl",
    };
    for (size_t candidateIndex = 0; candidateIndex < std::size(rendererCandidates); ++candidateIndex)
    {
        for (size_t infoIndex = 0; infoIndex < rendererInfos.size(); ++infoIndex)
        {
            if (rad::StrCaseEqual(rendererCandidates[candidateIndex], rendererInfos[infoIndex].name))
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
    m_renderer->Init(rendererIndex);

    m_guiContext = new sdl::GuiContext(this, m_renderer);

    // Load custom fonts
    float fontSize = GetDisplayDPI() / 72.0f * 12.0f;
#ifdef _WIN32
    m_guiContext->LoadFont("C:\\Windows\\Fonts\\consola.ttf", fontSize);
#endif

    return true;
}

bool WindowTest::OnEvent(const SDL_Event& event)
{
    if (m_guiContext)
    {
        m_guiContext->ProcessEvent(event);
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
    if (keyDown.keysym.sym == SDLK_F1)
    {
        m_showAboutWindow = !m_showAboutWindow;
    }
}

void WindowTest::OnKeyUp(const SDL_KeyboardEvent& keyUp)
{
    LogGlobal(Info, "OnKeyUp: %s", SDL_GetKeyName(keyUp.keysym.sym));
}

void WindowTest::OnMouseMove(const SDL_MouseMotionEvent& mouseMotion)
{
    LogGlobal(Info, "OnMouseMove: Pos=(%4d, %4d)", mouseMotion.x, mouseMotion.y);
}

void WindowTest::OnMouseButtonDown(const SDL_MouseButtonEvent& mouseButton)
{
    LogGlobal(Info, "OnMouseButtonDown: %s", GetMouseButtonName(mouseButton.button));
    if (mouseButton.button == SDL_BUTTON_LEFT)
    {
        m_circle.center = ImVec2(float(mouseButton.x), float(mouseButton.y));
        m_circle.color = m_randomColor(m_randomEngine);
        m_addCircle = true;
    }
    if (mouseButton.button == SDL_BUTTON_RIGHT)
    {
        if (!m_circles.empty())
        {
            m_circles.pop_back();
        }
    }
}

void WindowTest::OnMouseButtonUp(const SDL_MouseButtonEvent& mouseButton)
{
    LogGlobal(Info, "OnMouseButtonUp: %s", GetMouseButtonName(mouseButton.button));
    if (mouseButton.button == SDL_BUTTON_LEFT)
    {
        if (m_addCircle)
        {
            m_circles.push_back(m_circle);
            m_addCircle = false;
        }
    }
}

void WindowTest::OnMouseWheel(const SDL_MouseWheelEvent& mouseWheel)
{
    LogGlobal(Info, "OnMouseWheel: %+d", mouseWheel.y);
}

void WindowTest::OnRender()
{
    m_guiContext->StartFrame();

    if (m_showAboutWindow)
    {
        m_addCircle = false;
    }

    ImDrawList* bgDrawList = ImGui::GetBackgroundDrawList();
    for (const Circle& circle : m_circles)
    {
        bgDrawList->AddCircleFilled(circle.center, circle.radius, ImColor(circle.color));
    }
    if (m_addCircle)
    {
        int x = 0;
        int y = 0;
        if (SDL_GetMouseState(&x, &y) == SDL_BUTTON_LEFT)
        {
            m_circle.radius = sqrt(
                (float(x) - m_circle.center.x) * (float(x) - m_circle.center.x) +
                (float(y) - m_circle.center.y) * (float(y) - m_circle.center.y)
            );
        }
        bgDrawList->AddCircleFilled(m_circle.center, m_circle.radius, ImColor(m_circle.color));
    }

    if (m_showAboutWindow)
    {
        ImGui::ShowAboutWindow(&m_showAboutWindow);
    }

    m_guiContext->Render();

    m_renderer->SetScale(
        m_guiContext->GetIO().DisplayFramebufferScale.x,
        m_guiContext->GetIO().DisplayFramebufferScale.y
    );
    m_renderer->SetDrawColor(
        Uint8(m_clearColor.x * 255),
        Uint8(m_clearColor.y * 255),
        Uint8(m_clearColor.z * 255),
        Uint8(m_clearColor.w * 255)
    );
    m_renderer->Clear();
    m_guiContext->RenderDrawData();
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
