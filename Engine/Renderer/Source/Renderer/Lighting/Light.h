#pragma once

#include "glm/glm.hpp"

namespace Retro::Renderer {
	class Light {
	public:
		Light();
		~Light() = default;
		
		const glm::vec3& GetColor() const { return m_Color; }
		void SetColor(const glm::vec3& color) { m_Color = color; }

	private:
		glm::vec3 m_Color;
	};
}