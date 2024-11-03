#pragma once
#include <string>
#include <functional>

namespace HAIEngine
{
	class Event;

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(std::string_view title = "HAIEngine", uint32_t width = 1920, uint32_t height = 1080)
			:Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window() = default;
		Window(const Window&) = default;
		Window(Window&&) = default;
		Window& operator=(const Window&) = default;
		Window& operator=(Window&&) = default;
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

	};
}