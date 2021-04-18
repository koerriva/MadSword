#include "mspch.h"

#include "OpenGLContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace MadSword {
	OpenGLContext::OpenGLContext(GLFWwindow* glfwWindow):m_GLFWwindow(glfwWindow)
	{
		MS_CORE_ASSERT(glfwWindow, "窗口不存在");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_GLFWwindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MS_CORE_ASSERT(status, "Glad初始化失败");

		MS_CORE_INFO("OpenGL Context:");
		MS_CORE_INFO("\tVendor: {}", glGetString(GL_VENDOR));
		MS_CORE_INFO("\tRenderer: {}", glGetString(GL_RENDERER));
		MS_CORE_INFO("\tVersion: {}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_GLFWwindow);
	}
}