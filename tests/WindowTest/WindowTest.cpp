#include "WindowTest.h"

WindowTest::WindowTest()
{
    m_logger = rad::CreateLogger("WindowTest");
    RAD_LOG(m_logger, info, __func__);
}

WindowTest::~WindowTest()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnShown()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnHidden()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnExposed()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnMoved(int x, int y)
{
    RAD_LOG(m_logger, info, "OnMoved: x={:4}; y={:4};", x, y);
}

void WindowTest::OnResized(int width, int height)
{
    RAD_LOG(m_logger, info, "OnResized: width={:4}; height={:4};", width, height);
}

void WindowTest::OnPixelSizeChanged(int width, int height)
{
    RAD_LOG(m_logger, info, "OnPixelSizeChanged: width={:4}; height={:4};", width, height);
}

void WindowTest::OnMinimized()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnMaximized()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnRestored()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnMouseEnter()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnMouseLeave()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnFocusGained()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnFocusLost()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnCloseRequested()
{
    RAD_LOG(m_logger, info, __func__);
    Destroy();
}

void WindowTest::OnTakeFocus()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnHitTest()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnIccProfileChanged()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnDisplayChanged()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnDisplayScaleChanged()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnOccluded()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnEnterFullscreen()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnLeaveFullscreen()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnDestroyed()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnPenEnter()
{
    RAD_LOG(m_logger, info, __func__);
}

void WindowTest::OnPenLeave()
{
    RAD_LOG(m_logger, info, __func__);
}
