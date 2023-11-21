#include "LayerStack.hpp"
#include "hepch.hpp"

namespace HAIEngine
{
	LayerStack::LayerStack()
	{	
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin()+ m_layerInsertIndex, layer);
		m_layerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlayer)
	{
		m_layers.emplace_back(overlayer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_layerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlayer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlayer);
		if (it != m_layers.end())
			m_layers.erase(it);
	}
}