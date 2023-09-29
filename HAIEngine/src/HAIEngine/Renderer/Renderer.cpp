#include "hepch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace HAIEngine
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();

		auto m_OpenGLShader = std::dynamic_pointer_cast<OpenGLShader>(shader);

		m_OpenGLShader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		m_OpenGLShader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}