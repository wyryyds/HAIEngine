#include "OpenGLShader.hpp"
#include "hepch.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace HAIEngine
{
	static const std::unordered_map<std::string, GLenum> String2ShaderType = {
		{"vertex", GL_VERTEX_SHADER},
		{"fragment", GL_FRAGMENT_SHADER}
	};

	OpenGLShader::OpenGLShader(std::string_view filePath)
	{
		std::string sourceStr = ReadFile(filePath);
		auto shaderSources = PreProcess(sourceStr);
		ShaderCompile(shaderSources);
		// TODO change init Name;
		m_name = filePath;
	}

	OpenGLShader::OpenGLShader(std::string_view name, std::string_view vertexSrc, std::string_view fragmentSrc)
		: m_name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		ShaderCompile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_rendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_rendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(std::string_view name, const int value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.data());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(std::string_view name, const float value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.data());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(std::string_view name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.data());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(std::string_view name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.data());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(std::string_view name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.data());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(std::string_view name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.data());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(std::string_view name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.data());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(std::string_view filePath)
	{
		std::string result;
		std::ifstream in(filePath.data(), std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				HE_CORE_ERROR("Could not read from file '{0}'", filePath);
			}
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(std::string_view source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);


		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type{ source.substr(begin, eol - begin) };
			HE_CORE_ASSERT(type == "vertex" || type == "fragment", "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[String2ShaderType.find(type)->second] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::ShaderCompile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		auto program = glCreateProgram();

		std::vector<GLenum> glShaders;
		glShaders.reserve(shaderSources.size());
		for (auto& [shaderType, shaderSource] : shaderSources)
		{
			GLuint shader = glCreateShader(shaderType);

			const GLchar* source = (const GLchar*)shaderSource.data();
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				HE_CORE_ERROR("{0}", infoLog.data());
				HE_CORE_ASSERT(false, "shader compilation failure!");

				break;
			}
			glAttachShader(program, shader);
			glShaders.push_back(shader);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength > 0 ? maxLength : 512);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			
			for (auto shader : glShaders)
			{
				glDeleteShader(shader);
			}

			HE_CORE_ERROR("{0}", infoLog.data());
			HE_CORE_ASSERT(false, "Shader linked failure!");
			return;
		}

		for (auto shader : glShaders)
		{
			glDetachShader(program, shader);
		}

		m_rendererID = program;
	}
}