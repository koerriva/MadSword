#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace MadSword {
	class MS_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	Application* CreateApplication();
}
