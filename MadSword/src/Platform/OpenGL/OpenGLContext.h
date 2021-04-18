#pragma once

#include <MadSword/Renderer/GraphicsContext.h>

struct GLFWwindow;
namespace MadSword {
	class OpenGLContext :public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* glfwWindow);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_GLFWwindow;
	};
}