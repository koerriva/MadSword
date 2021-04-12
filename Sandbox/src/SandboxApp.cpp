#include <MadSword.h>
#include <MadSword/EntryPoint.h>

class Sandbox : public MadSword::Application {
public:
	Sandbox() {
		MS_TRACE("Sandbox startup");
		MS_INFO("Sandbox startup");
		MS_WARN("Sandbox startup");
		MS_ERROR("Sandbox startup");
		MS_CRITICAL("Sandbox startup");
	}
};

MadSword::Application* MadSword::CreateApplication() {
	return new Sandbox();
}