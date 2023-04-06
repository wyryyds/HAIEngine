#pragma once

#include "Core.h"

namespace HAIEngine
{
	class HE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	Application* CreateApplication();
}

