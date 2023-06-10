#pragma once
#include "RendererAPI.h"

namespace HAIEngine
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(glm::vec4 color = { 0.1f, 0.1f, 0.1f, 1 }) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexarray)
		{
			s_RendererAPI->DrawIndexed(vertexarray);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}