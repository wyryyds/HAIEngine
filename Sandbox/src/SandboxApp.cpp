#include"HAIEngine.h"

class ExampleLayer : public HAIEngine::Layer
{
public:
	ExampleLayer() :Layer("ExampleLayer")
	{

	}

	void OnUpdate()
	{
	}

	void OnEvent(HAIEngine::Event& event) override
	{
	}
};

class Sandbox : public HAIEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new HAIEngine::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};
HAIEngine::Application* HAIEngine::CreateApplication()
{
	return new Sandbox();
}