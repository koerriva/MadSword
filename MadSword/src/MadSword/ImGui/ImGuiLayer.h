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
		//���
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);
		//����
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);
		//����
		bool OnWindowResizeEvent(WindowResizeEvent& event);

		float m_Time=0;
	};
}

