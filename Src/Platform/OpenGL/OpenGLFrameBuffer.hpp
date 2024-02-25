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
		inline const uint32_t GetRenderID() const { return m_rendererID; }
		inline const uint32_t GetTextureID() const { return m_colorAttachnet; }
		inline const std::pair<uint32_t, uint32_t> GetSize() const { return std::make_pair(m_width, m_height); }

	private:
		uint32_t m_width, m_height;
		uint32_t m_rendererID;
		uint32_t m_colorAttachnet;
		uint32_t m_depthStencilBuffer;
	};

	// TODO merge
	class OpenGLMSAAFramebuffer : public FrameBuffer
	{
	public:
		OpenGLMSAAFramebuffer(uint32_t width, uint32_t height);
		virtual ~OpenGLMSAAFramebuffer();

		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		inline const uint32_t GetRenderID() const { return m_rendererID; }
		inline const uint32_t GetTextureID() const { return m_colorSampledAttachnet; }
		inline const std::pair<uint32_t, uint32_t> GetSize() const { return std::make_pair(m_width, m_height); }

	private:
		uint32_t m_width, m_height;
		uint32_t m_samples = 4;
		uint32_t m_rendererID;
		uint32_t m_colorSampledAttachnet;
		uint32_t m_depthStencilBuffer;
	};

	// TODO merge
	class OpenGLDepthMap : public FrameBuffer
	{
	public:
		OpenGLDepthMap();
		virtual ~OpenGLDepthMap();

		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		inline const uint32_t GetRenderID() const { return m_rendererID; }
		inline const uint32_t GetTextureID() const { return m_depthBuffer; }
		inline const std::pair<uint32_t, uint32_t> GetSize() const { return std::make_pair(m_width, m_height); }
		void UseDepthMap(uint32_t slot);

	private:
		const uint32_t m_width = 1024, m_height = 1024;
		uint32_t m_rendererID;
		uint32_t m_depthBuffer;
	};
}