#pragma once

#include "HAIEngine/Layer.hpp"
#include "HAIEngine/Events/KeyEvent.hpp"
#include "HAIEngine/Events/MouseEvent.hpp"
#include "HAIEngine/Application.hpp"

namespace HAIEngine
{
	class HE_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}

