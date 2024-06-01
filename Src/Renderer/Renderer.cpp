#include "Renderer.hpp"
#include "hepch.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>

namespace HAIEngine
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(glm::mat4 viewProjectionMatrix)
	{
		// glm::mat4 model = glm::mat4(1.0f);
		// model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

		m_SceneData->ViewProjectionMatrix = viewProjectionMatrix;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();

		auto openGLShader = std::dynamic_pointer_cast<OpenGLShader>(shader);

		openGLShader->UploadUniformMat4("u_viewProjection", m_SceneData->ViewProjectionMatrix);
		openGLShader->UploadUniformMat4("u_transform", transform);

		vertexArray->Bind();

		RenderCommand::Draw(vertexArray);
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::Draw(vertexArray);
	}

	void Renderer::GenerateShadow(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		auto openGLShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
		openGLShader->UploadUniformMat4("u_transform", transform);
		vertexArray->Bind();
		RenderCommand::Draw(vertexArray);
	}
}