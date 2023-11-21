#pragma once
#include "Core/Core.hpp"
#include "TimeStep.hpp"
#include <string>

namespace HAIEngine
{
	class Event;

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") : m_debugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_debugName; }

	protected:
		std::string m_debugName;
	};

}