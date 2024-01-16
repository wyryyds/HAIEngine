#pragma once
#include "Renderer/VertexArray.hpp"

#include <glm/glm.hpp>

namespace HAIEngine::RenderingSetting
{
	// OpenGL comparaison algorithm enum wrapper
	enum class EStencilFunc
	{
		NEVER         = 0x0200,
		LESS          = 0x0201,
		EQUAL         = 0x0202,
		LESS_EQUAL    = 0x0203,
		GREATER       = 0x0204,
		NOTEQUAL      = 0x0205,
		GREATER_EQUAL = 0x0206,
		ALWAYS        = 0x0207
	};
}

namespace HAIEngine 
{
	class RendererAPI 
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
		};

	public:
		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void Draw(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void DrawIndirctByVertices(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		virtual void EnableStencilTest() = 0;
		virtual void DisableStencilTest() = 0;
		virtual void EnableDepthTest() = 0;
		virtual void DisableDepthTest() = 0;

		virtual void SetStencilFunc(RenderingSetting::EStencilFunc func, int32_t ref, uint32_t mask) = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};

}