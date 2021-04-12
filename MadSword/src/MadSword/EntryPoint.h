#pragma once

#ifdef MS_PLATFORM_WINDOWS
extern MadSword::Application* MadSword::CreateApplication();

int main(int argc,char** argv) {
	auto app = MadSword::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif // MS_PLATFORM_WINDOWS
