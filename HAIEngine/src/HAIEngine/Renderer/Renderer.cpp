#include "hepch.hpp"
#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

#include <glm/gtx/string_cast.hpp>

namespace HAIEngine
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(Camera* camera)
	{
		// glm::mat4 model = glm::mat4(1.0f);
		// model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

		m_SceneData->ViewProjectionMatrix = camera->m_projection * camera->m_view;
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
		// 根据顶点索引绘制
		//RenderCommand::DrawIndexed(vertexArray);
		//RenderCommand::DrawIndirctByVertices(vertexArray);
		RenderCommand::Draw(vertexArray);
	}
}