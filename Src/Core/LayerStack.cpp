#include "LayerStack.hpp"
#include "Layer.hpp"
#include "hepch.hpp"

namespace HAIEngine
{
	LayerStack::~LayerStack()
	{
		for (const Layer* layer : m_layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin()+ m_layerInsertIndex, layer);
		m_layerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overLayer)
	{
		m_layers.emplace_back(overLayer);
	}

	void LayerStack::PopLayer(const Layer* layer)
	{
		auto it = std::ranges::find(m_layers, layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_layerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(const Layer* overLayer)
	{
		auto it = std::ranges::find(m_layers, overLayer);
		if (it != m_layers.end())
			m_layers.erase(it);
	}
}