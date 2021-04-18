#include "mspch.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"

namespace MadSword {
	Application* Application::s_Instance = nullptr;

	Application::Application(){
		MS_CORE_ASSERT(!s_Instance, "程序实例已存在");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(MS_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);

		const std::string vert = R"(
			#version 410 core
			layout(location=0) in vec3 position; 
			out vec4 vertColor;
			out vec3 vertPos;
			void main(){
				vertColor = vec4(1.);
				vertPos = position;
				gl_Position = vec4(position,1.0);
			}
		)";
		const std::string frag = R"(
			#version 410 core

			in vec3 vertPos;
			in vec4 vertColor;

			void main(){
				gl_FragColor = vertColor;
			}
		)";
		m_Shader.reset(new Shader(vert,frag));
	}
	Application::~Application(){}

	void Application::Run() {
		while (m_Running) {
			m_Window->ClearFramebuffer();

			for each (Layer * layer in m_LayerStack)
			{
				layer->OnUpdate();
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