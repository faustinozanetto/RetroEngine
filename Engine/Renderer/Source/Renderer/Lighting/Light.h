#pragma once

#include "glm/glm.hpp"

namespace Retro::Renderer
{
	class Light
	{
	public:
		Light();
		~Light() = default;

		const glm::vec3& GetColor() const { return m_Color; }
		void SetColor(const glm::vec3& color) { m_Color = color; }

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }

	private:
		glm::vec3 m_Color = {0.8f, 0.8f, 0.8f};
		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
	};
}
