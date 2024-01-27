#include "OpenGLTexture.hpp"
#include "hepch.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace HAIEngine
{
	OpenGLTexture2D::OpenGLTexture2D(std::string path)
		: m_path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.data(), &width, &height, &channels, 0);
		HE_CORE_ASSERT(data, "Faild To Load Image!");
		m_width = width;
		m_height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glBindTexture(GL_TEXTURE_2D, m_rendererID);
		glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		
		glBindTextureUnit(slot, m_rendererID);
	}

	OpenGLTexture3D::OpenGLTexture3D(std::string rtPath, std::string ltPath, std::string ttPath, std::string btPath, std::string backtPath, std::string ftPath)
		: m_facepaths{rtPath, ltPath, ttPath, btPath, backtPath, ftPath}
	{
		int width, height, channels;
		stbi_uc* data;
		stbi_set_flip_vertically_on_load(1);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glBindTexture(GL_TEXTURE_2D, m_rendererID);

		for (int i = 0; i < m_facepaths.size(); ++i)
		{
			stbi_uc* data = stbi_load(m_facepaths[i].c_str(), &width, &height, &channels, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// TODO
		m_height = height;
		m_width = width;
	}

	OpenGLTexture3D::~OpenGLTexture3D()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture3D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererID);
	}
}
