#pragma once

#include "HAIEngine/Layer.h"
#include "HAIEngine/Events/KeyEvent.h"
#include "HAIEngine/Events/MouseEvent.h"
#include "HAIEngine/Application.h"

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

