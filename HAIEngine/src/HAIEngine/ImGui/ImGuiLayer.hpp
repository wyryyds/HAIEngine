#pragma once

#include "Core/Layer.hpp"
#include "Core/Application.hpp"
#include "HAIEngine/Events/KeyEvent.hpp"
#include "HAIEngine/Events/MouseEvent.hpp"

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

