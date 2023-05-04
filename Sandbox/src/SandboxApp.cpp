#include"HAIEngine.h"

#include"imgui.h"
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
		ImGui::Begin("Test");
		ImGui::Text("hello world");
		ImGui::End();
		
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