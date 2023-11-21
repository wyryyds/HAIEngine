#pragma once
#include "Core.hpp"
#include "Events/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "Window.hpp"

namespace HAIEngine
{
	class ImGuiLayer;

	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_window; }
		inline static Application& Get()  { return *s_instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		static Application* s_instance;

		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_imguiLayer;
		LayerStack m_layerStack;
		bool m_running = true;
		float m_lastFrameTime = 0.0f;
	};

	Application* CreateApplication();
}

