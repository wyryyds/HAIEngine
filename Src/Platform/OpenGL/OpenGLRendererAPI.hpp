#pragma once
#include "Renderer/RendererAPI.hpp"

namespace HAIEngine
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;
		void SetClearColor(const glm::vec4& color = glm::vec4(0.1f, 0.1f, 0.1f, 1)) override;
		void Clear() override;
		void Draw(const std::shared_ptr<VertexArray>& vertexArray) override;
		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		void DrawIndirctByVertices(const std::shared_ptr<VertexArray>& vertexArray) override;

		void EnableStencilTest() override;
		void DisableStencilTest() override;
		void SetStencilFunc(RenderingSetting::EStencilFunc func, int32_t ref, uint32_t mask) override;

		void EnableDepthTest() override;
		void DisableDepthTest() override;

		void EnableBlend() override;
		void DisableBlend() override;
		void SetBlendFunc(RenderingSetting::EBlendFunc sfactor, RenderingSetting::EBlendFunc dfactor) override;
	};
}