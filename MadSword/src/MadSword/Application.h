#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include <MadSword/LayerStack.h>

namespace MadSword {
#define EVENT_BIND_FUNCTION(fn) std::bind(&fn,this,std::placeholders::_1)
//#define EVENT_BIND_FUNCTION(fn) [&](auto& EventFunc){ this->fn(EventFunc); }

	class MS_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}
