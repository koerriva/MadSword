#include "mspch.h"
#include "WindowsWindow.h"
#include <MadSword/Events/ApplicationEvent.h>
#include <MadSword/Events/KeyEvent.h>
#include <MadSword/Events/MouseEvent.h>
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

        glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowMovedEvent event(x, y);
            data.EventCallback(event);
            });
        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            if (focused==GLFW_TRUE) {
                WindowFocusEvent event;
                data.EventCallback(event);
            }
            else {
                WindowLostFocusEvent event;
                data.EventCallback(event);
            }
            });
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;
            WindowResizeEvent event(width,height);
            data.EventCallback(event);
            });
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
            });
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int keycode,int scancode, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS: {
                KeyPressedEvent event(keycode, 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(keycode);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(keycode, 2);
                data.EventCallback(event);
                break;
            }
            default:
                break;
            }
            });
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window,int btn,int action,int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(btn);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(btn);
                data.EventCallback(event);
                break;
            }
            default:
                break;
            }
            });
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event(xoffset, yoffset);
            data.EventCallback(event);
            });
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x,double y) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event(x, y);
            data.EventCallback(event);
            });
    }
    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }
}
