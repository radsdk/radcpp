#pragma once

#include "DirectMedia/Core.h"
#include "rad/Math/Random.h"

class WindowTest : public sdl::Window
{
public:
    WindowTest();
    ~WindowTest();

    bool Init();

    virtual bool OnEvent(const SDL_Event& event);

    virtual void OnShown() override;
    virtual void OnHidden() override;
    virtual void OnExposed() override;
    virtual void OnMoved(int x, int y) override;
    virtual void OnResized(int width, int height) override;
    virtual void OnMinimized() override;
    virtual void OnMaximized() override;
    virtual void OnRestored() override;
    virtual void OnEnter() override;
    virtual void OnLeave() override;
    virtual void OnKeyDown(const SDL_KeyboardEvent& keyDown) override;
    virtual void OnKeyUp(const SDL_KeyboardEvent& keyUp) override;
    virtual void OnMouseMove(const SDL_MouseMotionEvent& mouseMotion) override;
    virtual void OnMouseButtonDown(const SDL_MouseButtonEvent& mouseButton) override;
    virtual void OnMouseButtonUp(const SDL_MouseButtonEvent& mouseButton) override;
    virtual void OnMouseWheel(const SDL_MouseWheelEvent& mouseWheel) override;

    virtual void OnRender() override;

private:
    const char* GetMouseButtonName(Uint8 button);

    rad::Ref<sdl::Renderer> m_renderer;
    rad::Ref<sdl::GuiContext> m_guiContext;

    bool m_showAboutWindow = false;
    ImVec4 m_clearColor = ImVec4(0, 0, 0, 0);

    struct Circle
    {
        ImVec2 center;
        float radius;
        ImU32 color;
    };

    std::vector<Circle> m_circles;
    bool m_addCircle = false;
    Circle m_circle;
    std::default_random_engine m_randomEngine;
    std::uniform_int_distribution<ImU32> m_randomColor;

}; // class WindowTest
