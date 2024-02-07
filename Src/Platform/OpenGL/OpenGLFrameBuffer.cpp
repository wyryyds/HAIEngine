#include "OpenGLFrameBuffer.hpp"

#include <glad/glad.h>

namespace HAIEngine
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height)
		: m_width(width), m_height(height)
	{
		/* Generate OpenGL objects */
		glGenFramebuffers(1, &m_rendererID);
		glGenTextures(1, &m_colorAttachnet);
		glGenRenderbuffers(1, &m_depthStencilBuffer);

		/* Setup texture */
		glBindTexture(GL_TEXTURE_2D, m_colorAttachnet);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		/* Setup framebuffer */
		Bind();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_colorAttachnet, 0);
		UnBind();

		Resize(width, height);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(1, &m_colorAttachnet);
		glDeleteRenderbuffers(1, &m_depthStencilBuffer);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_width, m_height);
	}

	void OpenGLFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_width = width;
		m_height = height;
		/* Resize texture */
		glBindTexture(GL_TEXTURE_2D, m_colorAttachnet);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		/* Setup depth-stencil buffer (24 + 8 bits) */
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		/* Attach depth and stencil buffer to the framebuffer */
		Bind();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
		UnBind();
	}

	// TODO Test
	OpenGLMSAAFramebuffer::OpenGLMSAAFramebuffer(uint32_t width, uint32_t height)
	{
		/* Generate OpenGL objects */
		glGenFramebuffers(1, &m_rendererID);
		glGenTextures(1, &m_colorSampledAttachnet);
		glGenRenderbuffers(1, &m_depthStencilBuffer);

		/* Setup texture */
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorSampledAttachnet);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, m_width, m_height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		/* Setup framebuffer */
		Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_colorSampledAttachnet, 0);
		UnBind();

		Resize(width, height);
	}

	OpenGLMSAAFramebuffer::~OpenGLMSAAFramebuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(1, &m_colorSampledAttachnet);
		glDeleteRenderbuffers(1, &m_depthStencilBuffer);
	}

	void OpenGLMSAAFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_width, m_height);
	}

	void OpenGLMSAAFramebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLMSAAFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_width = width;
		m_height = height;
		/* Resize texture */
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorSampledAttachnet);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, m_width, m_height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		/* Setup depth-stencil buffer (24 + 8 bits) */
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		/* Attach depth and stencil buffer to the framebuffer */
		Bind();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
		UnBind();
	}
}