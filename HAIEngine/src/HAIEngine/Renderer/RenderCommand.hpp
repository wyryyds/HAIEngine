#pragma once
#include "RendererAPI.hpp"

namespace HAIEngine
{
	class RenderCommand
	{
	public:

		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(glm::vec4 color = { 0.1f, 0.1f, 0.1f, 1 }) 
		{ 
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear() 
		{ 
			s_RendererAPI->Clear();
		}

		inline static void Draw(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->Draw(vertexArray);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawIndirctByVertices(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndirctByVertices(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}