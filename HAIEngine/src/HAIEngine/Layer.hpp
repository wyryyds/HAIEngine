#pragma once
#include "HAIEngine/Core.hpp"
#include "HAIEngine/Core/TimeStep.hpp"
#include "HAIEngine/Events/Event.hpp"

namespace HAIEngine
{
	class HE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}