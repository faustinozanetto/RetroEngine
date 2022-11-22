#pragma once

#include "renderer/lighting/light.h"

namespace retro::renderer
{
	class point_light : public light
	{
	public:
		point_light();
		~point_light() override;

		const glm::vec3& get_position() const { return m_position; }
		void set_position(const glm::vec3& position) { m_position = position; }
		
		float& get_radius() { return m_radius; }
		void set_radius(float radius) { m_radius = radius; }

	private:
		glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
		float m_radius = 10.0f;
	};
}
