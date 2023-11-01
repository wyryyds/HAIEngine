#pragma once

#include "Core.hpp"
#include "HAIEngine/Events/Event.hpp"

namespace HAIEngine
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "HAIEngine", unsigned int width = 1920, unsigned int height = 1080)
			:Title(title), Width(width), Height(height)
		{

		}
	};

	class HE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event(&))>;

		virtual ~Window() 
		{

		};

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callbak) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());


	};
}