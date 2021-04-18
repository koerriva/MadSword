#include "mspch.h"
#include "WindowsInput.h"
#include "MadSword/Application.h"

#include <GLFW/glfw3.h>

namespace MadSword {
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyDownImpl(int keycode)
	{
		GLFWwindow* glfw_handle = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());
		auto status = glfwGetKey(glfw_handle, keycode);
		return status == GLFW_PRESS || status == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonDownImpl(int button)
	{
		GLFWwindow* glfw_handle = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());
		auto status = glfwGetKey(glfw_handle, button);
		return status == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		GLFWwindow* glfw_handle = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());
		double x=0, y=0;
		glfwGetCursorPos(glfw_handle, &x, &y);
		return std::pair<float, float>(static_cast<float>(x), static_cast<float>(y));
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return y;
	}


}