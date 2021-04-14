#include "mspch.h"
#include "Application.h"
#include "Log.h"

namespace MadSword {
	Application::Application(){
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(EVENT_BIND_FUNCTION(Application::OnEvent));
	}
	Application::~Application(){}

	void Application::Run() {

		while (m_Running) {
			m_Window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(EVENT_BIND_FUNCTION(Application::OnWindowCloseEvent));
		MS_TRACE(e);
	}
	bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}