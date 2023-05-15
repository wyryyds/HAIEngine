#include"hepch.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"
#include"glad/glad.h"

namespace HAIEngine
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_rendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_rendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) const
	{
		glBindVertexArray(m_rendererID);
		vertexBuffer->Bind();
	}
	void OpenGLVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) const
	{
	}
}