#include "Application.h"
#include "hepch.h"

#include "HAIEngine/Log.h"
#include "HAIEngine/Events/ApplicationEvent.h"
namespace HAIEngine
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

}