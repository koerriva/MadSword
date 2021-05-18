#pragma once

#include "MadSword/Layer.h"

namespace MadSword {
	class ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override;
		void OnDeAttach() override;
		void OnUpdate(Timestep deltaTime) override;
		void OnEvent(Event& e) override;

		void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time=0;
	};
}

