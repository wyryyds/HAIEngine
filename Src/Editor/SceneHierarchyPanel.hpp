#pragma once
#include "ECS/Scene.hpp"

namespace HAIEngine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);
		~SceneHierarchyPanel() = default;

		void SetContexts(const std::shared_ptr<Scene> scene);
		void OnImguiRender();
	private:
		std::shared_ptr<Scene> m_contexts;
		std::shared_ptr<GameObject> m_selectedContext;
	};
}