#pragma once

#include "Core.h"

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
