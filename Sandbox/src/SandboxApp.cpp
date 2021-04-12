#include <MadSword.h>
#include <MadSword/EntryPoint.h>

class Sandbox : public MadSword::Application {

};

MadSword::Application* MadSword::CreateApplication() {
	return new Sandbox();
}