#include "mspch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace MadSword {
	Application::Application(){
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application(){}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication)) {
			MS_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput)) {
			MS_TRACE(e);
		}


		while (m_Running) {
			m_Window->OnUpdate();
		}
	}
}