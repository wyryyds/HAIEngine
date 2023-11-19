#pragma once
#include <memory>

namespace HAIEngine
{
	class FrameBuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static std::shared_ptr<FrameBuffer> Create(uint32_t width, uint32_t height);
	};
}