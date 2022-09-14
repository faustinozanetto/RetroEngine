#pragma once

#include "Renderer/Shader/Shader.h"

namespace Retro::Renderer {
	class OpenGLShader : public Shader {
	public:
		/* Constructor & Destructor */
		OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
		~OpenGLShader() override;

		/* Methods */
		void Bind() override;
		void UnBind() override;

		void SetFloat(const std::string& uniform, float value) override;
		void SetVecFloat2(const std::string& uniform, glm::vec2 value) override;
		void SetVecFloat3(const std::string& uniform, glm::vec3 value) override;
		void SetVecFloat4(const std::string& uniform, glm::vec3 value) override;

		uint32_t GetUniformLocation(const std::string& uniform) override;


	};
}