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
    void WindowsWindow::ClearFramebuffer()
    {
        glViewport(0, 0, m_Data.Width, m_Data.Height);
        glClearColor(0.2, 0.3, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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
    void WindowsWindow::SetCursor(bool show)
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, show? GLFW_CURSOR_NORMAL:GLFW_CURSOR_HIDDEN);
    }
    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        MS_CORE_INFO("Creating Window {0},{1},{2}", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized) {
            int success = glfwInit();
            MS_CORE_ASSERT(success, "GLFW初始化失败");
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        int fw, fh;
        glfwGetFramebufferSize(m_Window, &fw, &fh);
        m_Data.FramebufferWidth = fw;
        m_Data.FramebufferHeight = fh;
        float xs, ys;
        glfwGetWindowContentScale(m_Window, &xs, &ys);
        m_Data.xscale = xs;
        m_Data.yscale = ys;

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        MS_CORE_ASSERT(status, "Glad初始化失败");

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

            int fw, fh;
            glfwGetFramebufferSize(window, &fw, &fh);
            data.FramebufferWidth = fw;
            data.FramebufferHeight = fh;
            float xs, ys;
            glfwGetWindowContentScale(window, &xs, &ys);
            data.xscale = xs;
            data.yscale = ys;

            WindowResizeEvent event(width,height,data.xscale,data.yscale);
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
