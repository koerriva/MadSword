#pragma once

#include "MadSword/Layer.h"

namespace MadSword {
	class MS_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override;
		void OnDeAttach() override;
		void OnUpdate() override;
		void OnEvent(Event& e) override;
	private:
		//Êó±ê
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);
		//¼üÅÌ
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);
		//´°¿Ú
		bool OnWindowResizeEvent(WindowResizeEvent& event);

		float m_Time=0;
	};
}

