#include"HAIEngine.h"

#include"imgui.h"

class ExampleLayer : public HAIEngine::Layer
{
public:
	ExampleLayer() :Layer("ExampleLayer")
	{

	}

	void OnUpdate()
	{
	}

	void OnImGuiRender()
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
	}

	~Sandbox()
	{

	}
};
HAIEngine::Application* HAIEngine::CreateApplication()
{
	return new Sandbox();
}