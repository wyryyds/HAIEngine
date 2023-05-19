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

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>&) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>&) = 0;

		static VertexArray* Create();
	};
}