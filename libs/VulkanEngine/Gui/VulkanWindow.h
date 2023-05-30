#ifndef VULKAN_WINDOW_H
#define VULKAN_WINDOW_H

#include "DirectMedia/Core.h"
#include "VulkanEngine/VulkanCore.h"

class VulkanWindow : public sdl::Window
{
public:
    VulkanWindow();
    ~VulkanWindow();

    bool Create(const char* title, int x, int y, int width, int height, Uint32 flags = 0);
    void Destroy();

    void SetInstance(rad::Ref<VulkanInstance> instance);
    VulkanInstance* GetInstance() { return m_instance.get(); }
    VkSurfaceKHR GetSurfaceHandle() const { return m_surface; }
    VulkanDevice* GetDevice() const { return m_device.get(); }

    void GetDrawableSize(int* width, int* height);

protected:
    virtual void OnClose();

    rad::Ref<VulkanInstance> m_instance;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    rad::Ref<VulkanDevice> m_device;

}; // class VulkanWindow

#endif // VULKAN_WINDOW_H
