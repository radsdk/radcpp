#include "WindowTest.h"
#include "rad/IO/Logging.h"

using namespace rad;

int main(int argc, char* argv[])
{
    Ref<sdl::Application> app = new sdl::Application();
    sdl::PerfCounter perfCounter;
    perfCounter.Start();
    if (!app->Init(argc, argv))
    {
        LogGlobal(Error, "Application::Init failed!");
        return -1;
    }
    perfCounter.Stop();
    LogGlobal(Info, "Application initialized (%fs elapsed).",
        perfCounter.GetElapsedInSeconds());

    Ref<WindowTest> window = new WindowTest();
    window->Init();

    return app->Run();
}
