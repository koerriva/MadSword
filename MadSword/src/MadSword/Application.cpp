#include "mspch.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"
#include <glad/glad.h>

namespace MadSword {
	Application* Application::s_Instance = nullptr;
	unsigned int vao;
	Application::Application(){
		MS_CORE_ASSERT(!s_Instance, "程序实例已存在");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(MS_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);
		float vertices[3 * 7] = {
			-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,
			0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,
			0.0f,0.5f,0.0f,0.0f,0.0f,1.0f,1.0f,
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices,9*sizeof(float)));

		BufferLayout layout = {
			{ShaderDataType::Vec3f,"position"},
			{ShaderDataType::Vec4f,"color"},
		};
		m_VertexBuffer->SetLayout(layout);

		unsigned int indices[3] = { 0,1,2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3 * sizeof(unsigned int)));

		glBindVertexArray(0);

		const std::string vert = R"(
			#version 410 core
			layout(location=0) in vec3 position; 
			layout(location=1) in vec4 color;

			out vec4 v_Color;
			out vec3 v_Pos;

			void main(){
				v_Color = color;
				v_Pos = position;
				gl_Position = vec4(position,1.0);
			}
		)";
		const std::string frag = R"(
			#version 410 core

			in vec3 v_Pos;
			in vec4 v_Color;

			void main(){
				gl_FragColor = v_Color;
			}
		)";
		m_Shader.reset(Shader::Create(vert,frag));
	}
	Application::~Application(){}

	void Application::Run() {
		while (m_Running) {
			m_Window->ClearFramebuffer();

			glBindVertexArray(vao);
			m_Shader->Bind();
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			m_Shader->UnBind();
			glBindVertexArray(0);

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