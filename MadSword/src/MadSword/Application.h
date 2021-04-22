#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include <MadSword/LayerStack.h>

#include "ImGui/ImGuiLayer.h"

namespace MadSword {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application& Instance() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		ImGuiLayer* m_ImGuiLayer;

		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
