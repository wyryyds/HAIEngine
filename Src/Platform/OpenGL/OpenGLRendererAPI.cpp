#include "OpenGLRendererAPI.hpp"
#include "hepch.hpp"

#include <glad/glad.h>

namespace HAIEngine
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRendererAPI::Draw(const std::shared_ptr<VertexArray>& vertexArray)
	{
		if (vertexArray->GetIndexBuffer() && vertexArray->GetIndexBuffer()->GetCount() > 0)
		{
			DrawIndexed(vertexArray);
		}
		else
		{
			DrawIndirctByVertices(vertexArray);
		}
		glBindVertexArray(0);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawIndirctByVertices(const std::shared_ptr<VertexArray>& vertexArray)
	{
		unsigned int count = 0;
		for (unsigned int i = 0; i < vertexArray->GetVertexBuffers().size(); ++i)
			count += vertexArray->GetVertexBuffers()[i]->GetSize();
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void OpenGLRendererAPI::EnableStencilTest()
	{
		glStencilMask(0xFF);
	}

	void OpenGLRendererAPI::DisableStencilTest()
	{
		glStencilMask(0x00);
	}

	void OpenGLRendererAPI::SetStencilFunc(RenderingSetting::EStencilFunc func, int32_t ref, uint32_t mask)
	{
		glStencilFunc(static_cast<GLenum>(func), ref, mask);
	}

	void OpenGLRendererAPI::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetDepthFunc(RenderingSetting::EDepthFunc func)
	{
		glDepthFunc(static_cast<GLenum>(func));
	}

	void OpenGLRendererAPI::EnableBlend()
	{
		glEnable(GL_BLEND);
	}

	void OpenGLRendererAPI::DisableBlend()
	{
		glDisable(GL_BLEND);
	}

	void OpenGLRendererAPI::SetBlendFunc(RenderingSetting::EBlendFunc sfactor, RenderingSetting::EBlendFunc dfactor)
	{
		glBlendFunc(static_cast<GLenum>(sfactor), static_cast<GLenum>(dfactor));
	}

}