#pragma once

#include "glm/glm.hpp"

namespace retro::renderer
{
	class light
	{
	public:
		light();
		virtual ~light();

		glm::vec4& get_color() { return m_color; }
		void set_color(const glm::vec4& color) { m_color = color; }

		const glm::vec3& get_position() const { return m_position; }
		void set_position(const glm::vec3& position) { m_position = position; }

	private:
		glm::vec4 m_color = {0.8f, 0.8f, 0.8f, 1.0f};
		glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
	};
}
