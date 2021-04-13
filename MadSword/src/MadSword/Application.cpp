#include "mspch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace MadSword {
	Application::Application(){}
	Application::~Application(){}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication)) {
			MS_TRACE(e.ToString());
		}
		if (e.IsInCategory(EventCategoryInput)) {
			MS_TRACE(e.ToString());
		}
		while (true);
	}
}