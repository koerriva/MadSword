#pragma once

#include "mspch.h"
#include "MadSword/Input.h"

namespace MadSword {
	class WindowsInput:public Input {
	protected:
		virtual bool IsKeyDownImpl(int keycode) override;
		virtual bool IsMouseButtonDownImpl(int button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual float GetMouseWheelImpl() override;
	};
}