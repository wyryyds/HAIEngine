#include "OpenGLFrameBuffer.hpp"

#include <glad/glad.h>

namespace HAIEngine
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height)
	{
		/* Generate OpenGL objects */
		glGenFramebuffers(1, &m_rendererID);
		glGenTextures(1, &m_textureID);
		glGenRenderbuffers(1, &m_depthStencilBuffer);

		/* Setup texture */
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		/* Setup framebuffer */
		Bind();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureID, 0);
		UnBind();

		Resize(width, height);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(1, &m_textureID);
		glDeleteTextures(1, &m_depthStencilBuffer);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
	}

	void OpenGLFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		/* Resize texture */
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		/* Setup depth-stencil buffer (24 + 8 bits) */
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		/* Attach depth and stencil buffer to the framebuffer */
		Bind();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
		UnBind();
	}
}