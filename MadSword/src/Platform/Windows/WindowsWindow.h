#pragma once

#include "MadSword/Window.h"
#include "glad/glad.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include <GLFW/glfw3native.h>
#include <MadSword/Renderer/GraphicsContext.h>
#include <Platform/OpenGL/OpenGLContext.h>


namespace MadSword {
	class WindowsWindow :public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		void ClearFramebuffer() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline unsigned int GetFramebufferWidth() const override { return m_Data.FramebufferWidth; }
		inline unsigned int GetFramebufferHeight() const override { return m_Data.FramebufferHeight; }

		void* GetNativeWindow() override { return m_Window; }
		void* GetPlatformHandle() override { return glfwGetWin32Window(m_Window); }

		inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void SetCursor(bool show) override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			unsigned int FramebufferWidth, FramebufferHeight;
			float xscale, yscale;
			bool VSync;
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}


