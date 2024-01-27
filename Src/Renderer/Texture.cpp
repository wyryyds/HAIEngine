#include "hepch.hpp"
#include "Texture.hpp"

#include "Renderer.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

namespace HAIEngine
{
	std::shared_ptr<Texture2D> Texture2D::Create(std::string path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}

		HE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Texture3D> Texture3D::Create(std::string rtPath, std::string ltPath, std::string ttPath, std::string btPath, std::string backtPath, std::string ftPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture3D>(rtPath, ltPath, ttPath, btPath, backtPath, ftPath);
		}

		HE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}