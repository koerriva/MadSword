#include "mspch.h"
#include "Application.h"
#include "Log.h"

namespace MadSword {
	Application* Application::s_Instance = nullptr;
	Application::Application(){
		MS_CORE_ASSERT(!s_Instance, u8"程序实例已存在");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(MS_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application() = default;

	void Application::Run() {
		Timer frameTimer;
		while (m_Running) {
			float time = frameTimer.GetLasTime();
			Timestep t = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			for each (Layer * layer in m_LayerStack)
			{
				layer->OnUpdate(t);
			}

			m_ImGuiLayer->Begin();
			for each (Layer * layer in m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(MS_BIND_EVENT_FN(Application::OnWindowClose));
		//MS_CORE_TRACE(e);
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.m_Handled) {
				break;
			}
		}
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
