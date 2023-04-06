#include"HAIEngine.h"

class Sandbox : public HAIEngine::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};
HAIEngine::Application* HAIEngine::CreateApplication()
{
	return new Sandbox();
}