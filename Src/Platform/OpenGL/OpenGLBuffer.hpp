#pragma once
#include "Renderer/Buffer.hpp"

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

		virtual const BufferLayout& GetLayout() const override { return m_layerout; };
		virtual void SetLayout(const BufferLayout& layout) override { m_layerout = layout; };

		inline virtual uint32_t GetSize() const override { return m_size; };

	private:
		uint32_t m_rendererID;
		uint32_t m_size;
		BufferLayout m_layerout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;

		inline virtual uint32_t GetCount() const override { return m_count; };

	private:
		uint32_t m_rendererID;
		uint32_t m_count;
	};
}