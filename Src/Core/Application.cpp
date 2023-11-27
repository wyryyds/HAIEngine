#include "Application.hpp"
#include "Core/Input.hpp"
#include "Core/Log.hpp"
#include "Core/TimeStep.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>


namespace HAIEngine
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	Application::Application(std::string_view name)
	{
		HE_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_window->SetVSync(false);

		Renderer::Init();

		m_imguiLayer = new ImGuiLayer();
		PushOverlay(m_imguiLayer);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		//输出事件信息
		//HE_CORE_INFO(e.ToString());
		for (auto it = m_layerStack.end();it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	void Application::Run()
	{
		while (m_running)
		{
			float time = glfwGetTime();
			HAIEngine::TimeStep timeStep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			for (Layer* layer : m_layerStack)
				layer->OnUpdate(timeStep);

			m_imguiLayer->Begin();
			for (Layer* layer : m_layerStack)
				layer->OnImGuiRender();
			m_imguiLayer->End();

			m_window->OnUpdate();
		}
	}

}