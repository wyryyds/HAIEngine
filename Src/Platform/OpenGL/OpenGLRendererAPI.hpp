#pragma once
#include "Renderer/RendererAPI.hpp"

namespace HAIEngine
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color = glm::vec4(0.1f, 0.1f, 0.1f, 1)) override;
		virtual void Clear() override;
		virtual void Draw(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void DrawIndirctByVertices(const std::shared_ptr<VertexArray>& vertexArray) override;

		virtual void EnableStencilTest() override;
		virtual void DisableStencilTest() override;
		virtual void EnableDepthTest() override;
		virtual void DisableDepthTest() override;

		virtual void SetStencilFunc(RenderingSetting::EStencilFunc func, int32_t ref, uint32_t mask) override;
	};
}