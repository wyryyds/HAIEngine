#pragma once
#include <vector>

namespace HAIEngine
{
	class Layer;

	class LayerStack
	{
	public:
		LayerStack() = default;
		LayerStack(const LayerStack&) = default;
		LayerStack& operator=(const LayerStack&) = default;
		LayerStack(LayerStack&&) = default;
		LayerStack& operator=(LayerStack&&) = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(const Layer* layer);
		void PopOverlay(const Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex = 0;
	};
}