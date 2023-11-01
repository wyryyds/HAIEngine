#pragma once

#include "Core.hpp"
#include "Window.hpp"
#include "LayerStack.hpp"
#include "HAIEngine/Events/ApplicationEvent.hpp"
#include "HAIEngine/ImGui/ImGuiLayer.hpp"

namespace HAIEngine
{
	class ImGuiLayer;

	class HE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get()  { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		static Application* s_Instance;

		float m_LastFrameTime = 0.0f;
	};

	Application* CreateApplication();
}

