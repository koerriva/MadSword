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

		void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time=0;
	};
}

