#include"HAIEngine.h"


class ExampleLayer : public HAIEngine::Layer
{
public:
	ExampleLayer() :Layer("ExampleLayer")
	{

	}

	void OnUpdate() override
	{
	}

	void OnImGuiRender() override
	{
		//堆栈溢出，待解决bug
		/*
		ImGui::Begin("Test");
		ImGui::Text("hello world");
		ImGui::End();
		*/
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