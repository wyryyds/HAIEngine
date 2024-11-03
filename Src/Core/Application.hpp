#pragma once
#include "Events/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "Window.hpp"

namespace HAIEngine
{
	class ImGuiLayer;

	class Application
	{
	public:
		Application(std::string_view name = "HAIEngine App");
		Application(const Application&) = delete;
		Application(Application&&) = default;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = default;
		virtual ~Application() = default;
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() const { return *m_window; }
		inline static Application& Get()  { return *s_instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		static Application* s_instance;

		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_imGuiLayer;
		LayerStack m_layerStack;
		bool m_running = true;
		double m_lastFrameTime = 0.0f;
	};

	Application* CreateApplication();
}

