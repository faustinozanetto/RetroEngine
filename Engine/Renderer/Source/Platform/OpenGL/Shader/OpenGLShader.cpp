#include "pch.h"

#include "Platform/OpenGL/Shader/OpenGLShader.h"
#include "glad/glad.h"

namespace Retro::Renderer {
	OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath) {
		// Parse both vertex and fragment shaders sources from file.
		auto vertexSource = ParseShaderContentsFromFile(vertexPath);
		auto fragmentSource = ParseShaderContentsFromFile(fragmentPath);
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
}