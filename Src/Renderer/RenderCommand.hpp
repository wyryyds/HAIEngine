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

		inline static void EnableStencilTest()
		{
			s_rendererAPI->EnableStencilTest();
		}

		inline static void DisableStencilTest()
		{
			s_rendererAPI->DisableStencilTest();
		}

		inline static void SetStencilFunc(RenderingSetting::EStencilFunc func, int32_t ref, uint32_t mask)
		{
			s_rendererAPI->SetStencilFunc(func, ref, mask);
		}

		inline static void EnableDepthTest()
		{
			s_rendererAPI->EnableDepthTest();
		}

		inline static void DisableDepthTest()
		{
			s_rendererAPI->DisableDepthTest();
		}

		inline static void EnableBlend()
		{
			s_rendererAPI->EnableBlend();
		}

		inline static void SetBlendFunc(RenderingSetting::EBlendFunc sfactor, RenderingSetting::EBlendFunc dfactor)
		{
			s_rendererAPI->SetBlendFunc(sfactor, dfactor);
		}

	private:
		static RendererAPI* s_rendererAPI;
	};
}