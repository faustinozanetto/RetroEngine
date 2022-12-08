#pragma once

#include "glm/glm.hpp"

namespace retro::renderer
{
	class light
	{
	public:
		light();
		virtual ~light();

		glm::vec3& get_color() { return m_color; }
		void set_color(const glm::vec3& color) { m_color = color; }

		float& get_intensity() { return m_intensity; }
		void set_intensity(float intensity) { m_intensity = intensity; }
	private:
		glm::vec3 m_color = { 0.8f, 0.8f, 0.8f };
		float m_intensity = 1.0f;
	};
}
