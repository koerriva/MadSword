#include "mspch.h"
#include "ImGuiLayer.h"
#include <Platform/OpenGL/ImGuiGLRenderer.h>
#include <GLFW/glfw3.h>
#include "MadSword/Application.h"

namespace MadSword {
	ImGuiLayer::ImGuiLayer():Layer("ImGuiLayer")
	{
		m_Time = (float) glfwGetTime();
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("data/fonts/Roboto.ttf", 16, nullptr,
			io.Fonts->GetGlyphRangesDefault());
		ImFontConfig fontConf;
		fontConf.OversampleH = 2;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("data/fonts/LiHeiPro.ttf", 16,&fontConf,
			io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
		io.Fonts->AddFontFromFileTTF("data/fonts/NotoSansSC.otf", 16, &fontConf,
			io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.BackendPlatformName = "imgui_impl_glfw";

		Application& app = Application::Instance();
		Window& window = app.GetWindow();
		io.ImeWindowHandle = window.GetNativeWindow();
		io.MouseDrawCursor = true;
		window.SetCursor(false);
		window.SetVSync(false);

		// Keyboard mapping. Dear ImGui will use those indices to peek into the io.KeysDown[] array.
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");

		MS_CORE_INFO("{} Attach!", GetName());
	}
	void ImGuiLayer::OnDeAttach()
	{
	}
	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Instance();
		Window& window = app.GetWindow();
		io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Begin(u8"ÖÐÎÄ×Ö·û²âÊÔ");
		ImGui::Text(u8"test Text²âÊÔ");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(EVENT_BIND_FUNCTION(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(EVENT_BIND_FUNCTION(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(EVENT_BIND_FUNCTION(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(EVENT_BIND_FUNCTION(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(EVENT_BIND_FUNCTION(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(EVENT_BIND_FUNCTION(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(EVENT_BIND_FUNCTION(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(EVENT_BIND_FUNCTION(ImGuiLayer::OnWindowResizeEvent));
	}
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = true;
		return false;
	}
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = false;
		return false;
	}
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(event.GetX(),event.GetY());
		return false;
	}
	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += event.GetX();
		io.MouseWheel += event.GetY();
		return false;
	}
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false;
	}
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event) 
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = false;

		return false;
	}
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = event.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000) {
			io.AddInputCharacter(keycode);
		}
		return false;
	}
	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(event.GetWidth(),event.GetHeight());
		return false;
	}
}
