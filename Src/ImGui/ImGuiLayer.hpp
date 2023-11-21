#pragma once
#include "Core/Layer.hpp"

namespace HAIEngine
{
	class ImGuiLayer :public Layer
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
		float m_time = 0.0f;
	};
}

