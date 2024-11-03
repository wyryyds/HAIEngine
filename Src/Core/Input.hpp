#pragma once

#include <utility>

namespace HAIEngine
{
	class Input
	{
	public:
		Input() = default;
		Input(const Input&) = default;
		Input&operator=(const Input&) = default;
		Input(Input&&) = default;
		Input&operator=(Input&&) = default;
		virtual ~Input() = default;
		
		inline static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressedImpl(keycode); }
		inline static bool IsKeyReleased(int keycode) { return s_instance->IsKeyReleasedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_instance->IsMouseButtonPressedImpl(button); }
		inline static bool IsMouseButtonReleased(int button) { return s_instance->IsMouseButtonReleasedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_instance->GetMouseYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsKeyReleasedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual bool IsMouseButtonReleasedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* s_instance;
	};

}