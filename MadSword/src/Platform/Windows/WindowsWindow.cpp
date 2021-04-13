#include "mspch.h"
#include "WindowsWindow.h"
//#include "MadSword/Core.h"

namespace MadSword {
    static bool s_GLFWInitialized;
    Window* Window::Create(const WindowProps& props) {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }
    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }
    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }
    void WindowsWindow::SetVSync(bool enabled)
    {
        m_Data.VSync = enabled;
        glfwSwapInterval(enabled ? 1 : 0);
    }
    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        MS_CORE_INFO("Creating Window {0},{1},{2}", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized) {
            int success = glfwInit();
            MS_CORE_ASSERT(success, "GLFW≥ı ºªØ ß∞‹");
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

    }
    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }
}
