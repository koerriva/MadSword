#pragma once
#include "Event.h"

namespace MadSword {
	class MS_API KeyEvent :public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		KeyEvent(const int keycode):m_KeyCode(keycode){}
		int m_KeyCode;
	};

	class MS_API KeyPressedEvent :public KeyEvent {
	public:
		KeyPressedEvent(const int keycode,uint16_t repeatCount):KeyEvent(keycode),m_RepeatCount(repeatCount){}
		uint16_t GetRepeatCount() const { return m_RepeatCount; }
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		uint16_t m_RepeatCount;
	};

	class MS_API KeyReleasedEvent :public KeyEvent {
	public:
		KeyReleasedEvent(const int keycode) :KeyEvent(keycode){}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class MS_API KeyTypedEvent :public KeyEvent {
	public:
		KeyTypedEvent(const int keycode) :KeyEvent(keycode) {}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
	};
}