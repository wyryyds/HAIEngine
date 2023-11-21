#pragma once

#include "Event.hpp"

#include <sstream>

namespace HAIEngine
{
	class KeyEvent :public Event
	{
	public:
		inline int GetKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard|EventCategoryInput)

	protected:
		KeyEvent(int keycode) : m_keyCode(keycode) {}

		int m_keyCode;
	};

	class KeyPressedEvent :public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_repeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_repeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent:" << m_keyCode << "(" << m_repeatCount << "repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_repeatCount;
	};

	class KeyRelaesedEvent :public KeyEvent
	{
	public:
		KeyRelaesedEvent(int keycode) :KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent:" << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypeEvent :public KeyEvent
	{
	public:
		KeyTypeEvent(int keycode) :KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypeEvent:" << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyType)
	};
}