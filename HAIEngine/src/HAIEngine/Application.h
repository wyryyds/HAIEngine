#pragma once

#include "Core.h"
#include "Window.h"
#include "HAIEngine/Events/ApplicationEvent.h"

namespace HAIEngine
{
	class HE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running=true;
	};

	Application* CreateApplication();
}

