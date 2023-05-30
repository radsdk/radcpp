#include "VulkanWindow.h"
#include "SDL2/SDL_vulkan.h"

VulkanWindow::VulkanWindow()
{
}

VulkanWindow::~VulkanWindow()
{
    if (m_handle)
    {
        Destroy();
    }
}

bool VulkanWindow::Create(const char* title, int x, int y, int width, int height, Uint32 flags)
{
    if (!sdl::Window::Create(title, x, y, width, height, flags | SDL_WINDOW_VULKAN))
    {
        return false;
    }

    if (!SDL_Vulkan_CreateSurface(m_handle, m_instance->GetHandle(), &m_surface))
    {
        LogVulkan(Error, "VulkanWindow: SDL_Vulkan_CreateSurface failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

void VulkanWindow::Destroy()
{
    if (m_surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(m_instance->GetHandle(), m_surface, nullptr);
        m_surface = VK_NULL_HANDLE;
    }
    Window::Destroy();
}

void VulkanWindow::SetInstance(rad::Ref<VulkanInstance> instance)
{
    m_instance = std::move(instance);
}

void VulkanWindow::GetDrawableSize(int* width, int* height)
{
    SDL_Vulkan_GetDrawableSize(m_handle, width, height);
}

void VulkanWindow::OnClose()
{
    m_shouldClose = true;
    Destroy();
}
