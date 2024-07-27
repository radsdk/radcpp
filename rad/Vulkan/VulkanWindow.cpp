#include <rad/Vulkan/VulkanWindow.h>
#include <SDL3/SDL_vulkan.h>

namespace rad
{

VulkanWindow::VulkanWindow()
{
}

VulkanWindow::VulkanWindow(Ref<VulkanContext> context) :
    m_context(std::move(context))
{
}

VulkanWindow::~VulkanWindow()
{
}

void VulkanWindow::SetVulkanContext(Ref<VulkanContext> context)
{
    m_context = std::move(context);
}

rad::Ref<VulkanSurface> VulkanWindow::CreateSurface()
{
    VkSurfaceKHR surfaceHandle = VK_NULL_HANDLE;
    VulkanInstance* instance = m_context->GetInstance();
    int result = SDL_Vulkan_CreateSurface(
        m_handle, instance->GetHandle(), nullptr, &surfaceHandle);
    if (result == 0)
    {
        return RAD_NEW VulkanSurface(m_context->GetInstance(), surfaceHandle);
    }
    else
    {
        RAD_LOG(GetVulkanLogger(), err, "SDL_Vulkan_CreateSurface failed: {}",
            SDL_GetError());
        return nullptr;
    }
}

bool VulkanWindow::OnEvent(const SDL_Event& event)
{
    return Window::OnEvent(event);
}

void VulkanWindow::OnResized(int width, int height)
{
}

void VulkanWindow::OnDestroyed()
{
    m_context->WaitIdle();
}

} // namespace rad
