#include "pch.h"

#include "directional_light.h"

namespace retro::renderer
{
	directional_light::directional_light()
	{
		set_direction(-35.0f, 60.0f);
	}

	void directional_light::set_direction(float azimuth, float elevation)
	{
		float az = glm::radians(azimuth);
		float el = glm::radians(elevation);

		m_direction.x = glm::sin(el) * glm::cos(az);
		m_direction.y = glm::cos(el);
		m_direction.z = glm::sin(el) * glm::sin(az);

		m_direction = glm::normalize(-m_direction);
	}
}
