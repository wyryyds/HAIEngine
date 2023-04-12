#include"HAIEngine.h"

class ExampleLayer : public HAIEngine::Layer
{
public:
	ExampleLayer() :Layer("ExampleLayer")
	{

	}

	void OnUpdate()
	{
		HE_INFO("ExampleLayer::Update");
	}

	void OnEvent(HAIEngine::Event& event) override
	{
		HE_TRACE(event.ToString());
	}
};

class Sandbox : public HAIEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};
HAIEngine::Application* HAIEngine::CreateApplication()
{
	return new Sandbox();
}