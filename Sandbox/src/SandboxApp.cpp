#include <MadSword.h>
#include <imgui.h>

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
		static bool show = true;
		ImGui::Begin(u8"≤‚ ‘",&show,ImGuiWindowFlags_DockNodeHost);
		ImGui::Text(u8"Hello wrold!ƒ„∫√ ¿ΩÁ!");
		ImGui::End();
	}
};

class Sandbox : public MadSword::Application {
public:
	Sandbox() {
		MS_TRACE("Sandbox startup");
		MS_INFO("Sandbox startup");
		MS_WARN("Sandbox startup");
		MS_ERROR("Sandbox startup");
		MS_CRITICAL("Sandbox startup");

		PushLayer(new DebugLayer());
	}

	~Sandbox() {

	}
};

MadSword::Application* MadSword::CreateApplication() {
	return new Sandbox();
}