#pragma once

#include "Shapes.h"

class RandomColor
{
public:
    using RandomEngine = std::default_random_engine;
    using Dist = std::uniform_real_distribution<float>;
    RandomColor()
    {
        m_engineR.seed('R');
        m_engineG.seed('G');
        m_engineB.seed('B');
        m_engineA.seed('A');
        SetRangeR(0.0f, 1.0f);
        SetRangeG(0.0f, 1.0f);
        SetRangeB(0.0f, 1.0f);
        SetRangeA(1.0f, 1.0f);
    }
    ~RandomColor()
    {
    }

    void SetRangeR(float min, float max) { m_randR = Dist(min, max); }
    void SetRangeG(float min, float max) { m_randG = Dist(min, max); }
    void SetRangeB(float min, float max) { m_randB = Dist(min, max); }
    void SetRangeA(float min, float max) { m_randA = Dist(min, max); }

    ImColor Generate()
    {
        return ImColor(
            m_randR(m_engineR),
            m_randG(m_engineG),
            m_randB(m_engineB),
            m_randA(m_engineA)
        );
    }

    RandomEngine m_engineR;
    Dist m_randR;
    RandomEngine m_engineG;
    Dist m_randG;
    RandomEngine m_engineB;
    Dist m_randB;
    RandomEngine m_engineA;
    Dist m_randA;
}; // class RandomColor

extern RandomColor g_randColor;

class Brush : public rad::RefCounted<Brush>
{
public:
    Brush(ShapeList* shapes) :
        m_shapes(shapes) {}
    virtual ~Brush() {}

    virtual void OnKeyDown(const SDL_KeyboardEvent& keyDown) {}
    virtual void OnKeyUp(const SDL_KeyboardEvent& keyUp) {}
    virtual void OnMouseMove(const SDL_MouseMotionEvent& mouseMotion) {}
    virtual void OnMouseButtonDown(const SDL_MouseButtonEvent& mouseButton) {}
    virtual void OnMouseButtonUp(const SDL_MouseButtonEvent& mouseButton) {}
    virtual void OnMouseWheel(const SDL_MouseWheelEvent& mouseWheel) {}
    virtual void OnRender(sdl::Renderer* renderer, sdl::GuiContext* gui) {}

    ShapeList* m_shapes;

}; // class Brush

class CircleBrush : public Brush
{
public:
    CircleBrush(ShapeList* shapes) :
        Brush(shapes) {}
    ~CircleBrush() {}

    virtual void OnMouseButtonDown(const SDL_MouseButtonEvent& mouseButton)
    {
        if (mouseButton.button == SDL_BUTTON_LEFT)
        {
            m_circle.center = ImVec2(float(mouseButton.x), float(mouseButton.y));
            m_circle.color = g_randColor.Generate();
            m_addCircle = true;
        }

        if (mouseButton.button == SDL_BUTTON_RIGHT)
        {
            if (!m_shapes->circles.empty())
            {
                m_shapes->circles.pop_back();
            }
        }
    }

    virtual void OnMouseButtonUp(const SDL_MouseButtonEvent& mouseButton)
    {
        if (mouseButton.button == SDL_BUTTON_LEFT)
        {
            if (m_addCircle && m_circle.radius > 0.0f)
            {
                m_shapes->circles.push_back(m_circle);
                m_addCircle = false;
            }
        }
    }

    virtual void OnRender(sdl::Renderer* renderer, sdl::GuiContext* gui)
    {
        ImDrawList* bgDrawList = ImGui::GetBackgroundDrawList();
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
            Draw(bgDrawList, m_circle);
        }
    }

    bool m_addCircle = false;
    Circle m_circle;

}; // class CircleBrush
