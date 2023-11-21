#pragma once
#include "Event.hpp"

#include <sstream>

namespace HAIEngine
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x,const float y) :m_mouseX(x), m_mouseY(y) {}

		inline float GetX()const { return m_mouseX; }
		inline float GetY()const { return m_mouseY; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent:" << m_mouseX << "," << m_mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_mouseX, m_mouseY;
	};

	class MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(const float xOffset,const float yOffset) :m_xOffset(xOffset), m_yOffset(yOffset) {}

		inline float GetXOffset() const { return m_xOffset; }
		inline float GetYOffset() const { return m_yOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent:" << GetXOffset() << "," << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_xOffset, m_yOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		constexpr int GetMouseButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput |EventCategoryMouseButton)

	protected:
		MouseButtonEvent(int button) :m_button(button) {}

		int m_button;
		int a = GetMouseButton();
	};

	class MouseButtonPressedEvent :public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) :MouseButtonEvent(button) {}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed:" << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleaseEvent :public MouseButtonEvent
	{
	public:
		MouseButtonReleaseEvent(int button) :MouseButtonEvent(button) {}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent:" << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}