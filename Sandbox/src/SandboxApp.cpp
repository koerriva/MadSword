#include "SandboxApp.h"

class DebugLayer : public MadSword::Layer {
public:
	DebugLayer():Layer("DebugLayer"){};
	void OnUpdate() override {
		//MS_TRACE("DebugLayer:Update");
	}

	void OnEvent(MadSword::Event& e) override {
		//MS_INFO("DebugLayer:Event {}", e);
	}

	void OnImGuiRender() override {
		ImGuiContext* ctx = ImGui::GetCurrentContext();
		static bool show = true;
		ImGui::Begin(u8"²âÊÔ",&show,ImGuiWindowFlags_DockNodeHost);
		ImGui::Text(u8"Hello wrold!ÄãºÃÊÀ½ç!");
		ImGui::End();
	}
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

	}
	void OnDeAttach() override {
		m_NodeEditor->Save();

		ImNodes::DestroyContext();
	}
	void OnEvent(MadSword::Event& e) override {
		
	}
	void OnImGuiRender() override {
		m_NodeEditor->Show();
	}
private:
	std::unique_ptr<App::NodeEditor> m_NodeEditor;
};

class Sandbox : public MadSword::Application {
public:
	Sandbox() {
		MS_INFO("Sandbox startup");

		PushLayer(new NodeEditorLayer());
	}

	~Sandbox() {

	}
};

MadSword::Application* MadSword::CreateApplication() {
	return new Sandbox();
}