#pragma once
#include "HAIEngine.hpp"
#include "Core/Layer.hpp"

#include "SceneHierarchyPanel.hpp"
#include "ECS/Components/MeshFilter.hpp"

namespace HAIEngine
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		void CheckInput(TimeStep ts);
	
	private:
		ShaderLibrary m_ShaderLibrary;

		EditorCamera* m_PerspectiveCamera;
		std::unique_ptr<CameraController> m_CameraController;

		glm::vec3 m_LightCorlor{ 1.0f, 1.0f, 1.0f };
		glm::vec3 m_CubeColor{ 1.0f, 0.5f, 0.31f };
		glm::vec3 lightPos{ 1.2f, 2.0f, 2.0f };
		int m_Specuness = 64;

		std::shared_ptr<VertexBuffer> m_SuareVB, m_LightVB;
		std::shared_ptr<VertexArray> m_SquareVA, m_LightVA;

		std::shared_ptr<Texture2D> m_Texture, m_specularTexture;

		std::shared_ptr<FrameBuffer> m_frameBuffer;

		float lastMouseX, lastMouseY;
		bool IsReControlMouse = false;
		glm::vec2 m_viewportSize = { 1920, 1080 };

		//sample ECS
		std::shared_ptr<Scene> scene;
		SceneHierarchyPanel m_panel;

		 MeshFilter m_meshFilter;
		 MeshRenderer m_meshRenderer;
	};
}