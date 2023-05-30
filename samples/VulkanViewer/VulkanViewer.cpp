#include "VulkanViewer.h"
#include "rad/IO/Logging.h"

using namespace rad;

VulkanViewer::VulkanViewer()
{
    LogGlobal(Info, "VulkanViewer::VulkanViewer()");
}

VulkanViewer::~VulkanViewer()
{
    LogGlobal(Info, "VulkanViewer::~VulkanViewer()");
}

bool VulkanViewer::Init()
{
    float windowScale = sdl::Application::GetInstance()->GetDisplayDPI(0) / 96.0f;
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    Create("VulkanViewer",
        SDL_WINDOWPOS_CENTERED_DISPLAY(0), SDL_WINDOWPOS_CENTERED_DISPLAY(0),
        int(800 * windowScale), int(600 * windowScale),
        windowFlags);

    Ref<VulkanPhysicalDevice> gpuSelected;
    std::vector<Ref<VulkanPhysicalDevice>> gpus = m_instance->EnumeratePhysicalDevices();
    if (gpus.empty())
    {
        LogVulkan(Critical, "No Vulkan device available!");
        return false;
    }

    gpuSelected = gpus[0];
    for (const auto& gpu : gpus)
    {
        const VkPhysicalDeviceProperties& props = gpu->GetProperties();

        // Prefer the first discrete GPU.
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            gpuSelected = gpu;
            break;
        }
    }

    if (gpuSelected)
    {
        LogVulkan(Info, "Vulkan device selected: %s (0x%X)",
            gpuSelected->GetProperties().deviceName,
            gpuSelected->GetProperties().deviceID);
    }

    // Create the logical device
    m_device = gpuSelected->CreateDevice();
    LogVulkan(Info, "Vulkan logical device created: handle=0x%p", m_device->GetHandle());

    m_frame = new VulkanFrame(this);

    int width = 0;
    int height = 0;
    GetDrawableSize(&width, &height);

    m_renderTarget = m_device->CreateImage2DRenderTarget(VK_FORMAT_R8G8B8A8_UNORM,
        width, height, VK_IMAGE_USAGE_SAMPLED_BIT);
    m_depthStencil = m_device->CreateImage2DDepthStencil(VK_FORMAT_D32_SFLOAT_S8_UINT, width, height);

    m_gui = new VulkanGuiContext(this);
    VulkanGuiContext::InitInfo guiInfo = {};
    VulkanSwapchain* swapchain = m_frame->GetSwapchain();
    guiInfo.swapchainImageCount = swapchain->GetImageCount();
    m_gui->Init(guiInfo);
    float fontSize = GetDisplayDPI() / 72.0f * 12.0f;
#if defined(_WIN32)
    auto fonts = m_gui->GetFonts();
    fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", fontSize);
    m_gui->UploadFonts();
#endif

    m_frame->SetColorBuffer(m_renderTarget);
    m_frame->SetOverlay(m_gui->GetRenderTarget());

    return true;
}

bool VulkanViewer::OnEvent(const SDL_Event& event)
{
    m_gui->ProcessEvent(event);
    return Window::OnEvent(event);
}

void VulkanViewer::OnShown()
{
    LogGlobal(Info, "OnShown");
}

void VulkanViewer::OnHidden()
{
    LogGlobal(Info, "OnHidden");
}

void VulkanViewer::OnExposed()
{
    LogGlobal(Info, "OnExposed");
}

void VulkanViewer::OnMoved(int x, int y)
{
    LogGlobal(Info, "OnMoved: %4d, %4d", x, y);
}

void VulkanViewer::OnResized(int width, int height)
{
    m_device->WaitIdle();

    GetDrawableSize(&width, &height);

    m_renderTarget = m_device->CreateImage2DRenderTarget(VK_FORMAT_R8G8B8A8_UNORM,
        width, height, VK_IMAGE_USAGE_SAMPLED_BIT);
    m_depthStencil = m_device->CreateImage2DDepthStencil(VK_FORMAT_D32_SFLOAT_S8_UINT, width, height);

    m_frame->Resize(width, height);
    m_gui->Resize(width, height);
    m_frame->SetColorBuffer(m_renderTarget);
    m_frame->SetOverlay(m_gui->GetRenderTarget());

    LogGlobal(Info, "OnResized: %4d, %4d", width, height);
}

void VulkanViewer::OnMinimized()
{
    LogGlobal(Info, "OnMinimized");
}

void VulkanViewer::OnMaximized()
{
    LogGlobal(Info, "OnMaximized");
}

void VulkanViewer::OnRestored()
{
    LogGlobal(Info, "OnRestored");
}

void VulkanViewer::OnEnter()
{
    LogGlobal(Info, "OnEnter");
}

void VulkanViewer::OnLeave()
{
    LogGlobal(Info, "OnLeave");
}

void VulkanViewer::OnKeyDown(const SDL_KeyboardEvent& keyDown)
{
    LogGlobal(Info, "OnKeyDown: %s", SDL_GetKeyName(keyDown.keysym.sym));
}

void VulkanViewer::OnKeyUp(const SDL_KeyboardEvent& keyUp)
{
    LogGlobal(Info, "OnKeyUp: %s", SDL_GetKeyName(keyUp.keysym.sym));
}

void VulkanViewer::OnMouseMove(const SDL_MouseMotionEvent& mouseMotion)
{
}

void VulkanViewer::OnMouseButtonDown(const SDL_MouseButtonEvent& mouseButton)
{
    LogGlobal(Info, "OnMouseButtonDown: %s", GetMouseButtonName(mouseButton.button));
}

void VulkanViewer::OnMouseButtonUp(const SDL_MouseButtonEvent& mouseButton)
{
    LogGlobal(Info, "OnMouseButtonUp: %s", GetMouseButtonName(mouseButton.button));
}

void VulkanViewer::OnMouseWheel(const SDL_MouseWheelEvent& mouseWheel)
{
    LogGlobal(Info, "OnMouseWheel: %+d", mouseWheel.y);
}

void VulkanViewer::OnRender()
{
    m_frame->BeginFrame();
    m_gui->StartFrame();
    ImGui::ShowDemoWindow();
    m_gui->Render();
    m_gui->Submit();
    m_frame->EndFrame();
}

const char* VulkanViewer::GetMouseButtonName(Uint8 button)
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
