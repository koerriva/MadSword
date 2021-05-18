#pragma once

#include "Core.h"
#include <utility>

namespace MadSword {
	class Input {
	public:
		static bool IsKeyDown(int keycode) { return s_Instance->IsKeyDownImpl(keycode); }
		static bool IsMouseButtonDown(int button) { return s_Instance->IsMouseButtonDownImpl(button); }
		static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		static float GetMouseWheel() { return s_Instance->GetMouseWheelImpl(); }
	protected:
		virtual bool IsKeyDownImpl(int keycode) = 0;
		virtual bool IsMouseButtonDownImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual float GetMouseWheelImpl() = 0;
	private:
		static Input* s_Instance;
	};
}