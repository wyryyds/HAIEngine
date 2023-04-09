#include "Application.h"
#include "Hzpch.h"

#include "HAIEngine/Log.h"
#include "HAIEngine/Events/ApplicationEvent.h"
namespace HAIEngine
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			HE_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			HE_TRACE(e);
		}

		while (true);
	}

}