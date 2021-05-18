#pragma once
#include "MadSword/Layer.h"

namespace Example
{
	class Example01Layer:public MadSword::Layer
	{
	public:
		Example01Layer() :Layer("Example01Layer") {}

		void OnAttach() override;
		void OnUpdate(MadSword::Timestep deltaTime) override;
		void OnEvent(MadSword::Event& e) override;
	};
}
