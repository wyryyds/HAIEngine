#pragma once
#include "HAIEngine/Window.h"
#include "GLFW/glfw3.h"

namespace HAIEngine
{
	class WindowsWindow :public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
	};
}