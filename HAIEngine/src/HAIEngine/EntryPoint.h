#pragma once

#ifdef  HE_PLATFORM_WINDOWS

extern HAIEngine::Application* HAIEngine::CreateApplication();

int main(int argc, char** argv)
{
	HAIEngine::Log::Init();
	HE_CORE_WARN("Init Log");
	auto app = HAIEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif //  HE_PLATFORM_WINDOWS

