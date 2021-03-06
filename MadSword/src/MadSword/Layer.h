#pragma once

#include "Events/Event.h"
#include <MadSword/Events/MouseEvent.h>
#include <MadSword/Events/ApplicationEvent.h>
#include <MadSword/Events/KeyEvent.h>

#include "Core/Timestep.h"

namespace MadSword {
	class Layer {
	public:
		Layer(const std::string& name = "Layer") :m_DebugName(name) {};
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDeAttach() {}
		virtual void OnUpdate(Timestep deltaTime) {}
		virtual void OnEvent(Event& e);

		virtual void OnImGuiRender() {};

		inline std::string& GetName() { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}
