#pragma once
#include "TimeStep.hpp"
#include <string>

namespace HAIEngine
{
	class Event;

	class Layer
	{
	public:
		Layer(std::string_view name = "Layer") : m_debugName(name) {}
		Layer(const Layer& other) = default;
		Layer(Layer&& other) = default;
		Layer& operator=(const Layer& other) = default;
		Layer& operator=(Layer&& other) = default;
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline std::string_view GetName() const { return m_debugName; }

	protected:
		std::string m_debugName;
	};

}