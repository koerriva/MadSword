#include <MadSword.h>

class DebugLayer : public MadSword::Layer {
public:
	DebugLayer(const std::string& name = "DebugLayer") {};
	void OnUpdate() override {
		MS_TRACE("DebugLayer:Update");
	}

	void OnEvent(MadSword::Event& e) override {
		MS_INFO(e);
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