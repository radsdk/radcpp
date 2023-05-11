#include "WindowTest.h"
#include "rad/IO/Logging.h"

using namespace rad;

int main(int argc, char* argv[])
{
    Ref<sdl::Application> app = new sdl::Application();
    if (!app->Init(argc, argv))
    {
        LogGlobal(Error, "Application::Init failed!");
        return -1;
    }

    Ref<WindowTest> window = new WindowTest();
    window->Init();

    return app->Run();
}
