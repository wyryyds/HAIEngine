#pragma once
#include "Core/Layer.hpp"

namespace HAIEngine
{
	class ImGuiLayer : public Layer
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
		struct ImVec3 { float x, y, z; ImVec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) { x = _x; y = _y; z = _z; } };
		void ImGuiTheme(ImVec3 textColor, ImVec3 headColor, ImVec3 areaColor, ImVec3 bodyColor, ImVec3 popsColor);
		void SetImGuiStyle();
	private:
		float m_time = 0.0f;
	};
}

