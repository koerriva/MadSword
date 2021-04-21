#include "mspch.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"
#include <glad/glad.h>

namespace MadSword {
	Application* Application::s_Instance = nullptr;
	Application::Application(){
		MS_CORE_ASSERT(!s_Instance, "����ʵ���Ѵ���");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(MS_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//m_VertexArray.reset(VertexArray::Create());
		m_SquadVA.reset(VertexArray::Create());

		float vertices[4 * 7] = {
			-0.5f,-0.5f,0.0f,1.0f,1.0f,0.0f,1.0f,
			0.5f,-0.5f,0.0f,0.0f,1.0f,1.0f,1.0f,
			0.5f,0.5f,0.0f,1.0f,0.0f,1.0f,1.0f,
			-0.5f,0.5f,0.0f,1.0f,0.0f,1.0f,1.0f,
		};
		//m_VertexBuffer.reset(VertexBuffer::Create(vertices,sizeof(vertices)));
		std::shared_ptr<VertexBuffer> m_SquadVB;
		m_SquadVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ShaderDataType::Vec3f,"position"},
			{ShaderDataType::Vec4f,"color"},
		};
		//m_VertexBuffer->SetLayout(layout);
		//m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_SquadVB->SetLayout(layout);
		m_SquadVA->AddVertexBuffer(m_SquadVB);

		unsigned int indices[6] = { 0,1,2,0,2,3 };
		//m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));
		//m_VertexArray->AddIndexBuffer(m_IndexBuffer);
		std::shared_ptr<IndexBuffer> m_SquadEB;
		m_SquadEB.reset(IndexBuffer::Create(indices, sizeof(indices)));
		m_SquadVA->AddIndexBuffer(m_SquadEB);

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

			m_Shader->Bind();
			//m_VertexArray->Bind();
			//glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			m_SquadVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquadVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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