#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

#include "glm/glm.hpp"

namespace Retro::Renderer {
	class Shader : public GraphicsObject {
	public:
		/* Destructor */
		virtual ~Shader() = default;

		/* Methods */
		virtual void SetFloat(const std::string& uniform, float value) = 0;
		virtual void SetVecFloat2(const std::string& uniform, glm::vec2 value) = 0;
		virtual void SetVecFloat3(const std::string& uniform, glm::vec3 value) = 0;
		virtual void SetVecFloat4(const std::string& uniform, glm::vec4 value) = 0;

		virtual unsigned int GetUniformLocation(const std::string& uniform) = 0;

		/* Instaintiate */
		static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);
	};
}