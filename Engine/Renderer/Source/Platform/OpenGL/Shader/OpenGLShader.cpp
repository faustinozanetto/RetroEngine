#include "pch.h"

#include "Platform/OpenGL/Shader/OpenGLShader.h"
#include "glad/glad.h"

namespace Retro::Renderer {
	OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath) {
		Logger::Line();
		// Parse both vertex and fragment shaders sources from file.
		const std::string& vertexSource = ParseShaderContentsFromFile(vertexPath);
		const std::string& fragmentSource = ParseShaderContentsFromFile(fragmentPath);
		// Assign shader sources.
		m_ShaderSources = ProcessShaderSources(vertexSource, fragmentSource);
		// Compile shader.
		CompileShader();
		Logger::Line();
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ObjectHandle);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_ObjectHandle);
	}

	void OpenGLShader::UnBind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetFloat(const std::string& uniform, float value)
	{
	}

	void OpenGLShader::SetVecFloat2(const std::string& uniform, glm::vec2 value)
	{
	}

	void OpenGLShader::SetVecFloat3(const std::string& uniform, glm::vec3 value)
	{
	}

	void OpenGLShader::SetVecFloat4(const std::string& uniform, glm::vec3 value)
	{
	}

	uint32_t OpenGLShader::GetUniformLocation(const std::string& uniform)
	{
		return 0;
	}

	std::string OpenGLShader::ParseShaderContentsFromFile(const std::string& shaderFilePath)
	{
		Logger::Info("OpenGLShader::ParseShaderContentsFromFile | Parsing: " + shaderFilePath);
		std::string parsedContents = "";
		std::ifstream in(shaderFilePath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			const size_t size = in.tellg();
			// Valid size.
			if (size != -1)
			{
				parsedContents.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&parsedContents[0], size);
				in.close();
			} else{
				Logger::Error("OpenGLShader::ParseShaderContentsFromFile | An error occurred.");
			}
		}
		else {
			Logger::Error("OpenGLShader::ParseShaderContentsFromFile | An error occurred.");
		}
		return parsedContents;
	}

	std::map<ShaderType, std::string> OpenGLShader::ProcessShaderSources(const std::string& vertexSource, const std::string& fragmentSource)
	{
		std::map<ShaderType, std::string> shaderSources = {};
		shaderSources[ShaderType::Fragment] = fragmentSource;
		shaderSources[ShaderType::Vertex] = vertexSource;

		return shaderSources;
	}

	void OpenGLShader::CompileShader()
	{
		uint32_t shaderProgram = glCreateProgram();
		std::array<GLenum, 2> glShaderIDs{};
		int shaderIndex = 0;
		// Iterate through the shader sources.
		for (auto& shader : m_ShaderSources) {
			const ShaderType shaderType = shader.first;
			const std::string& shaderSource = shader.second;

			// Creating the program
			const GLuint openGLShader = glCreateShader(shaderType);
			// Assigning source
			const GLchar* sourceCStr = shaderSource.c_str();
			glShaderSource(openGLShader, 1, &sourceCStr, 0);

			// Compiling the shader
			glCompileShader(openGLShader);

			// Error handling
			GLint success = 0;
			glGetShaderiv(openGLShader, GL_COMPILE_STATUS, &success);
			if (success == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(openGLShader, GL_INFO_LOG_LENGTH, &maxLength);

				// Get error log.
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(openGLShader, maxLength, &maxLength, &infoLog[0]);

				// Delete the shader.
				glDeleteShader(openGLShader);

				// Print error log.
				Logger::Error("OpenGLShader::CompileShader | Shader compilation failed: " + std::string(infoLog.begin(), infoLog.end()));
				break;
			}

			// Attach
			glAttachShader(shaderProgram, openGLShader);
			glShaderIDs[shaderIndex] = openGLShader;
			shaderIndex++;
		}
		// Assign program ID
		m_ObjectHandle = shaderProgram;

		// Link the program
		glLinkProgram(m_ObjectHandle);

		// Error handling
		GLint success = 0;
		glGetProgramiv(m_ObjectHandle, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ObjectHandle, GL_INFO_LOG_LENGTH, &maxLength);

			// Getting the error
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ObjectHandle, maxLength, &maxLength, &infoLog[0]);

			// Delete the program
			glDeleteProgram(m_ObjectHandle);

			// Delete shaders.
			for (const auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			// Print error log.
			Logger::Error("OpenGLShader::Compile | Shader linking failed: " + std::string(infoLog.begin(), infoLog.end()));

			return;
		}

		Logger::Info("OpenGLShader::Compile | Shader Compiled Successfully!");

		// Cleanup.
		for (const auto id : glShaderIDs)
		{
			glDetachShader(m_ObjectHandle, id);
			glDeleteShader(id);
		}
	}
}