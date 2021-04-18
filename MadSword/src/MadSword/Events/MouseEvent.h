#pragma once
#include "Event.h"

namespace MadSword {
	class MouseMovedEvent :public Event {
	public:
		MouseMovedEvent(const float x, const float y) :m_MouseX(x), m_MouseY(y) {};
		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }
		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse|EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent :public Event {
	public:
		MouseScrolledEvent(const float x, const float y) :m_OffsetX(x), m_OffsetY(y) {};
		float GetX() const { return m_OffsetX; }
		float GetY() const { return m_OffsetY; }
		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_OffsetX << ", " << m_OffsetY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_OffsetX, m_OffsetY;
	};

	class MouseButtonEvent :public Event {
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput|EventCategoryMouseButton)
	protected:
		MouseButtonEvent(const int button):m_Button(button){}
		int m_Button;
	};

	class MouseButtonPressedEvent :public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(const int button):MouseButtonEvent(button){}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent :public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(const int button):MouseButtonEvent(button){}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}