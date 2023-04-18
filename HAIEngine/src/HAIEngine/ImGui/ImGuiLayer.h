#pragma once

#include"HAIEngine/Layer.h"


namespace HAIEngine
{
	class ImGuiLayer :public Layer
	{
	public :
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
	};
}

