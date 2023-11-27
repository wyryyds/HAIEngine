#include "Renderer.hpp"
#include "Renderer/Shader.hpp"
#include "hepch.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"

namespace HAIEngine
{
	std::shared_ptr<Shader> Shader::Create(std::string_view filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filePath);
		}
		HE_CORE_ASSERT(false, "UnKnown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(std::string_view name, std::string_view vertexSrc, std::string_view fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
		HE_CORE_ASSERT(false, "UnKnown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::AddShaderWithName(std::string_view name, const std::shared_ptr<Shader>& shader)
	{
		HE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name.data()] = shader;
	}

	void ShaderLibrary::AddShader(const std::shared_ptr<Shader>& shader)
	{
		auto name = shader->GetName();
		AddShaderWithName(name, shader);
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(std::string_view filePath)
	{
		auto shader = Shader::Create(filePath);
		AddShader(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(std::string_view name, std::string_view filePath)
	{
		auto shader = Shader::Create(filePath);
		AddShaderWithName(name, shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(std::string_view name)
	{
		HE_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name.data()];
	}

	bool ShaderLibrary::Exists(std::string_view name) const
	{
		return m_Shaders.find(name.data()) != m_Shaders.end();
	}
}
