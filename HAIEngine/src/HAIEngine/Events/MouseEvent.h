#pragma once

#include"Event.h"

#include<sstream>

namespace HAIEngine
{
	class HE_API MouseMovedEvent :public Event
	{
	public:
		MouseMovedEvent(float x, float y) :m_MouseX(x), m_MouseY(y) 
		{

		}

		inline float GetX()const { return m_MouseX; }
		inline float GetY()const { return m_MouseY; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent:" << m_MouseX << "," << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_MouseX, m_MouseY;
	};

	class HE_API MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) :m_XOffset(xOffset), m_YOffset(yOffset)
		{

		}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent:" << GetXOffset() << "," << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};

	class HE_API MouseButtonEvent : public Event
	{
	public:
		inline int GetNouseButton()const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button) :m_Button(button)
		{

		}

		int m_Button;
	};

	class HE_API MouseButtonPressed :public MouseButtonEvent
	{
	public:
		MouseButtonPressed(int button) :MouseButtonEvent(button)
		{

		}
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed:" << m_Button;
			return ss.str();
		}
	};

	class HE_API MouseButtonReleaseEvent :public MouseButtonEvent
	{
	public:
		MouseButtonReleaseEvent(int button) :MouseButtonEvent(button) 
		{

		}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent:" << m_Button;
			return ss.str();
		}
	};
}