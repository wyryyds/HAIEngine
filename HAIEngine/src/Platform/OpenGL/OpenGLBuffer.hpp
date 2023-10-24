#pragma once
#include "HAIEngine/Renderer/Buffer.hpp"

#include "glad/glad.h"

namespace HAIEngine
{

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layerout; };
		virtual void SetLayout(const BufferLayout& layout) override { m_Layerout = layout; };

		inline virtual uint32_t GetSize() const override { return m_Size; };

	private:
		uint32_t m_RendererID;
		uint32_t m_Size;
		BufferLayout m_Layerout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;

		inline virtual uint32_t GetCount() const override { return m_Count; };
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}