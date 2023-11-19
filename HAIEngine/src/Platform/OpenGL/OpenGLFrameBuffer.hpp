#pragma once
#include "HAIEngine/Renderer/FrameBuffer.hpp"

namespace HAIEngine
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(uint32_t width, uint32_t height);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void UnBind() override;
		void Resize(uint32_t width, uint32_t height);
		inline const uint32_t GetTextureID() const { return m_textureID; }
	private:
		uint32_t m_rendererID;
		uint32_t m_textureID;
		uint32_t m_depthStencilBuffer;
	};
}