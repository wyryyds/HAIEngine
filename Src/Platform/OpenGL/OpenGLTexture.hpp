#pragma once
#include "Renderer/Texture.hpp"

namespace HAIEngine 
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(std::string path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::string m_path;
		uint32_t m_width, m_height;
		uint32_t m_rendererID;
	};

	class OpenGLTexture3D : public Texture3D
	{
	public:
		OpenGLTexture3D(std::string rtPath, std::string ltPath, std::string ttPath, std::string btPath, std::string backtPath, std::string ftPath);
		virtual ~OpenGLTexture3D();

		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::vector<std::string> m_facepaths;
		uint32_t m_width, m_height;
		uint32_t m_rendererID;
	};
}