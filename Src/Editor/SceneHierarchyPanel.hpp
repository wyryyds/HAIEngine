#pragma once
#include "ECS/Scene.hpp"

namespace HAIEngine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);
		SceneHierarchyPanel(const SceneHierarchyPanel&) = default;
		SceneHierarchyPanel(SceneHierarchyPanel&&) = default;
		SceneHierarchyPanel& operator=(const SceneHierarchyPanel&) = default;
		SceneHierarchyPanel& operator=(SceneHierarchyPanel&&) = default;
		~SceneHierarchyPanel() = default;

		void SetContexts(const std::shared_ptr<Scene>& scene);
		void OnImGuiRender();
		void ShowProperties() const;

	private:
		std::shared_ptr<Scene> m_contexts;
		std::shared_ptr<GameObject> m_selectedContext;
	};
}