#include "VulkanViewer.h"
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

    if (!app->LoadVulkanLibrary())
    {
        LogGlobal(Error, "Cannot find Vulkan loader!");
        return -1;
    }

    Ref<VulkanInstance> instance = new VulkanInstance();
    std::set<std::string> extensionNames = app->GetVulkanInstanceExtensions();
    instance->Init(
        "VulkanViewer",
        VK_MAKE_VERSION(0, 0, 0),
        extensionNames
    );

    Ref<VulkanViewer> window = new VulkanViewer();
    window->SetInstance(instance);
    window->Init();

    return app->Run();
}
