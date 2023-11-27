#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>

namespace HAIEngine
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual	void Unbind() const = 0;

		virtual std::string_view GetName() const = 0;

		static std::shared_ptr<Shader> Create(std::string_view filePath);
		static std::shared_ptr<Shader> Create(std::string_view name, std::string_view vertexSrc, std::string_view fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void AddShaderWithName(std::string_view name, const std::shared_ptr<Shader>& shader);
		void AddShader(const std::shared_ptr<Shader>& shader);
		std::shared_ptr<Shader> Load(std::string_view filePath);
		std::shared_ptr<Shader> Load(std::string_view name, std::string_view filePath);
		std::shared_ptr<Shader> Get(std::string_view name);

		bool Exists(std::string_view name) const;
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};
}