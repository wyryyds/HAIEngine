#pragma once
#include "Renderer/Shader.hpp"
#include <glm/glm.hpp>

// TODO Remove
typedef unsigned int GLenum;

namespace HAIEngine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string_view filePath);
		OpenGLShader(std::string_view name, std::string_view vertexSrc, std::string_view fragmentSrc);
		virtual ~OpenGLShader() ;

		virtual void Bind() const override;
		virtual	void Unbind() const override;

		virtual std::string_view GetName() const override { return m_name; };

		void UploadUniformInt(std::string_view name, const int value);

		void UploadUniformFloat(std::string_view name, const float value);
		void UploadUniformFloat2(std::string_view name, const glm::vec2& value);
		void UploadUniformFloat3(std::string_view name, const glm::vec3& value);
		void UploadUniformFloat4(std::string_view name, const glm::vec4& value);

		void UploadUniformMat3(std::string_view name, const glm::mat3& matrix);
		void UploadUniformMat4(std::string_view name, const glm::mat4& matrix);

	private:
		std::string ReadFile(std::string_view filePath);
		std::unordered_map<GLenum, std::string> PreProcess(std::string_view source);
		void ShaderCompile(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		std::string m_name;
		uint32_t m_rendererID;
	};
}