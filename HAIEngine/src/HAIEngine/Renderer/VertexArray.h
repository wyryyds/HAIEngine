#pragma once
#include"HAIEngine/Renderer/Buffer.h"

namespace HAIEngine
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>&) const = 0;
		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>&) const = 0;

		static VertexArray* Create();
	};
}