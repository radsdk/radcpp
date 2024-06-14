#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <rad/Gui/Application.h>
#include "WindowTest.h"

rad::Ref<rad::Application> g_app;
rad::Ref<WindowTest> g_window;

int SDL_AppInit(void** appState, int argc, char** argv)
{
    rad::InitLogging("WindowTest.log", true);
    auto guiLogger = rad::GetGuiLogger();
    g_app = new rad::Application();
    if (!g_app->Init(argc, argv))
    {
        RAD_LOG(guiLogger, err, "Application::Init failed!");
        return SDL_APP_FAILURE;
    }
    g_window = new WindowTest();
    g_window->Create("WindowTest", 800, 600,
        SDL_WindowFlags(SDL_WINDOW_RESIZABLE));
    return SDL_APP_CONTINUE;
}

int SDL_AppIterate(void* appState)
{
    g_app->OnIdle();
    return g_app->GetExit() ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}

int SDL_AppEvent(void* appState, const SDL_Event* event)
{
    g_app->OnEvent(*event);
    return g_app->GetExit() ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appState)
{
    g_window.reset();
    g_app->Destroy();
    g_app.reset();
}
