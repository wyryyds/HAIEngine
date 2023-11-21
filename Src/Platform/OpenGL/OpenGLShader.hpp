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
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader() ;

		virtual void Bind() const override;
		virtual	void Unbind() const override;

		virtual const std::string& GetName() const override { return m_name; };

		void UploadUniformInt(const std::string& name, const int value);

		void UploadUniformFloat(const std::string& name, const float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void ShaderCompile(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		std::string m_name;
		uint32_t m_rendererID;
	};
}