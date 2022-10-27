#pragma once

#include "renderer/lighting/light.h"

namespace retro::renderer
{
	class directional_light : public light
	{
	public:
		directional_light();
		~directional_light() = default;

		const glm::vec3& get_direction() const { return m_direction; }
		void set_direction(const glm::vec3& direction) { m_direction = direction; }

	private:
		glm::vec3 m_direction;
	};
}
