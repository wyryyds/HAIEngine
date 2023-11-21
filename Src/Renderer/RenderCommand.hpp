#pragma once
#include "RendererAPI.hpp"

namespace HAIEngine
{
	class RenderCommand
	{
	public:

		inline static void Init()
		{
			s_rendererAPI->Init();
		}

		inline static void SetClearColor(glm::vec4 color = { 0.1f, 0.1f, 0.1f, 1 }) 
		{ 
			s_rendererAPI->SetClearColor(color);
		}

		inline static void Clear() 
		{ 
			s_rendererAPI->Clear();
		}

		inline static void Draw(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_rendererAPI->Draw(vertexArray);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_rendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawIndirctByVertices(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_rendererAPI->DrawIndirctByVertices(vertexArray);
		}
	private:
		static RendererAPI* s_rendererAPI;
	};
}