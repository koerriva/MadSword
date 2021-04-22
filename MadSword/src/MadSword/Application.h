#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include <MadSword/LayerStack.h>

#include "ImGui/ImGuiLayer.h"
#include <MadSword/Renderer/Shader.h>
#include <MadSword/Renderer/Buffer.h>
#include <MadSword/Renderer/VertexArray.h>
#include <MadSword/Renderer/OrthographicCamera.h>

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

		static inline Application& Instance() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		ImGuiLayer* m_ImGuiLayer;

		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<VertexArray> m_SquadVA;
		std::shared_ptr<OrthographicCamera> m_Camera;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
