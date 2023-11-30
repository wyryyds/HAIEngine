#include "SceneHierarchyPanel.hpp"
#include "ECS/GameObject.hpp"

#include <imgui.h>
namespace HAIEngine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene)
	{
		SetContexts(scene);
	}

	void SceneHierarchyPanel::SetContexts(const std::shared_ptr<Scene> scene)
	{
		m_contexts = scene;
	}

	void SceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("Hierarchy");
;
		for (auto& gameObject : m_contexts->m_gameObjects)
		{
			ImGuiTreeNodeFlags flags = ((m_selectedContext == gameObject) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx(gameObject->m_name.c_str(), flags);
			if (ImGui::IsItemClicked())
			{
				m_selectedContext = gameObject;
			}
			if (opened)
				ImGui::TreePop();
		}

		

		ImGui::End();

	}
}