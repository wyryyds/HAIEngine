#pragma once
#include "Renderer/FrameBuffer.hpp"

namespace HAIEngine
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(uint32_t width, uint32_t height);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		void Invalidate();
		inline const uint32_t GetTextureID() const { return m_colorAttachnet; }

	private:
		uint32_t m_width, m_height;
		uint32_t m_rendererID;
		uint32_t m_colorAttachnet;
		uint32_t m_depthStencilBuffer;
	};
}