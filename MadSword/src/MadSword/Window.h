#pragma once
#include "mspch.h"
#include <MadSword/Events/Event.h>

namespace MadSword {
	struct WindowProps {
		std::string Title;
		unsigned int Width, Height;

		WindowProps(const std::string& title="MadSword",
			unsigned int width=1280,
			unsigned int height=720):Title(title),Width(width),Height(height){}
	};

	class MS_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {
			MS_CORE_TRACE("Window close");
		};
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//…Ë÷√ Ù–‘
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}