#include "SandboxApp.h"

#include "MadSword/Renderer/Renderer.h"

class DebugLayer : public MadSword::Layer {
public:
	DebugLayer():Layer("DebugLayer"){}
	void OnUpdate() override {
		//MS_TRACE("DebugLayer:Update");
	}

	void OnEvent(MadSword::Event& e) override {
		//MS_INFO("DebugLayer:Event {}", e);
	}

	void OnImGuiRender() override {
		ImGuiContext* ctx = ImGui::GetCurrentContext();
		static bool show = true;
		ImGui::Begin(u8"≤‚ ‘",&show,ImGuiWindowFlags_DockNodeHost);
		ImGui::Text(u8"Hello wrold!ƒ„∫√ ¿ΩÁ!");
		ImGui::End();
	}
};

class ExampleLayer : public MadSword::Layer
{
public:
	ExampleLayer():Layer("ExampleLayer"){}

	void OnAttach() override
	{
		m_Camera.reset(new MadSword::OrthographicCamera(-2, 2, -2, 2));
		m_SquadVA.reset(MadSword::VertexArray::Create());

		float vertices[4 * 7] = {
			-0.5f,-0.5f,0.0f,1.0f,1.0f,0.0f,1.0f,
			0.5f,-0.5f,0.0f,0.0f,1.0f,1.0f,1.0f,
			0.5f,0.5f,0.0f,1.0f,0.0f,1.0f,1.0f,
			-0.5f,0.5f,0.0f,1.0f,0.0f,1.0f,1.0f,
		};
		std::shared_ptr<MadSword::VertexBuffer> m_SquadVB;
		m_SquadVB.reset(MadSword::VertexBuffer::Create(vertices, sizeof(vertices)));

		const MadSword::BufferLayout layout = {
			{MadSword::ShaderDataType::Vec3f,"position"},
			{MadSword::ShaderDataType::Vec4f,"color"},
		};
		m_SquadVB->SetLayout(layout);
		m_SquadVA->AddVertexBuffer(m_SquadVB);

		unsigned int indices[6] = { 0,1,2,0,2,3 };
		std::shared_ptr<MadSword::IndexBuffer> m_SquadEB;
		m_SquadEB.reset(MadSword::IndexBuffer::Create(indices, sizeof(indices)));
		m_SquadVA->AddIndexBuffer(m_SquadEB);

		const std::string vert = R"(
			#version 410 core
			layout(location=0) in vec3 position; 
			layout(location=1) in vec4 color;

			uniform mat4 VP;
			uniform mat4 M;
		
			out vec4 v_Color;
			out vec3 v_Pos;

			void main(){
				v_Color = color;
				v_Pos = position;
				gl_Position = VP * vec4(position,1.0);
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
		m_Shader.reset(MadSword::Shader::Create(vert, frag));
	}

	float t = 0.0f;
	void OnUpdate() override
	{
		t++;
		{
			MadSword::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.1f, 1.0f });
			MadSword::RenderCommand::Clear();

			m_Camera->SetPosition(vec3(0.0f, 0.0f, -1.0f));
			m_Camera->SetRotation(vec3(0.0f, 0.0f, abs(sin(t))));

			MadSword::Renderer::BeginScene(m_Camera);
			MadSword::Renderer::Submit(m_Shader, m_SquadVA);
			MadSword::Renderer::EndScene();
		}
	}

	void OnEvent(MadSword::Event& e) override
	{
		
	}
private:
	std::shared_ptr<MadSword::Shader> m_Shader;
	std::shared_ptr<MadSword::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<MadSword::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<MadSword::VertexArray> m_VertexArray;

	std::shared_ptr<MadSword::VertexArray> m_SquadVA;
	std::shared_ptr<MadSword::OrthographicCamera> m_Camera;
};

class NodeEditorLayer : public MadSword::Layer {
public:
	NodeEditorLayer() {
		m_NodeEditor.reset(new App::NodeEditor());
	}
	void OnAttach() override {
		ImNodes::CreateContext();
		ImGuiContext* ctx = ImGui::GetCurrentContext();
		if (ctx == nullptr) {
			MS_ERROR("ImGui Context not found");
		}
		ImNodes::SetImGuiContext(ctx);
		ImNodes::StyleColorsDark();
		ImNodesIO& io = ImNodes::GetIO();
		ImNodesStyle& style = ImNodes::GetStyle();
		ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
		ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

		//m_NodeEditor->Load();
		App::NodeEditorLoad();
	}
	void OnDeAttach() override {
		//m_NodeEditor->Save();

		//App::NodeEditorSave();
		ImNodes::DestroyContext();
	}
	void OnEvent(MadSword::Event& e) override {
		
	}
	void OnImGuiRender() override {
		//m_NodeEditor->Show();
		
		App::NodeEditorShow();
	}
private:
	std::unique_ptr<App::NodeEditor> m_NodeEditor;
};

class Sandbox : public MadSword::Application {
public:
	Sandbox() {
		MS_INFO("Sandbox startup");

		PushLayer(new NodeEditorLayer());

		PushLayer(new ExampleLayer());
	}

	~Sandbox() override {
		MS_INFO("Sandbox shutdown");
	}
};

MadSword::Application* MadSword::CreateApplication() {
	return new Sandbox();
}