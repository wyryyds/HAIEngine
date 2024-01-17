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

	enum class EBlendFunc
	{
		GL_ZERO                = 0,
        GL_ONE                 = 1,
        GL_SRC_COLOR           = 0x0300,
        GL_ONE_MINUS_SRC_COLOR = 0x0301,
        GL_SRC_ALPHA           = 0x0302,
        GL_ONE_MINUS_SRC_ALPHA = 0x0303,
        GL_DST_ALPHA           = 0x0304,
        GL_ONE_MINUS_DST_ALPHA = 0x0305,
        GL_DST_COLOR           = 0x0306,
        GL_ONE_MINUS_DST_COLOR = 0x0307,
        GL_SRC_ALPHA_SATURATE  = 0x0308
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
		virtual void SetStencilFunc(RenderingSetting::EStencilFunc func, int32_t ref, uint32_t mask) = 0;

		virtual void EnableDepthTest() = 0;
		virtual void DisableDepthTest() = 0;

		virtual void EnableBlend() = 0;
		virtual void SetBlendFunc(RenderingSetting::EBlendFunc sfactor, RenderingSetting::EBlendFunc dfactor) = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};

}