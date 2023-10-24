#pragma once

#include "HAIEngine/Renderer/RendererAPI.hpp"

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
	};
}