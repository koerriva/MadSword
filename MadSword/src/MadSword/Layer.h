#pragma once

#include "Events/Event.h"

namespace MadSword {
	class MS_API Layer {
	public:
		Layer(const std::string& name = "Layer") :m_DebugName(name) {};
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDeAttach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& e);

		inline std::string& GetName() { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}