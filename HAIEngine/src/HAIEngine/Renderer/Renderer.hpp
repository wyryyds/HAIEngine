#pragma once

#include "RenderCommand.hpp"
#include "EditorCamera.hpp"
#include "Shader.hpp"

namespace HAIEngine
{
	class Renderer
	{
	public:

		static void Init();
		static void BeginScene(EditorCamera* camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};
}