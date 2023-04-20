#pragma once

#include "HAIEngine/Layer.h"
#include "HAIEngine/Events/KeyEvent.h"
#include "HAIEngine/Events/MouseEvent.h"
#include "HAIEngine/Application.h"


namespace HAIEngine
{
	class HE_API ImGuiLayer :public Layer
	{
	public :
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleaseEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyTypeEvent(KeyTypeEvent& e);
		bool OnKeyRealeasedEvent(KeyRelaesedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float m_Time=0.0f;
	};
}

