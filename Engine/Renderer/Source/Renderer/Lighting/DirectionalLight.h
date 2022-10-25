#pragma once

#include "Renderer/Lighting/Light.h"

namespace Retro::Renderer
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		~DirectionalLight() = default;

		const glm::vec3& GetDirection() const { return m_Direction; }
		void SetDirection(const glm::vec3& direction) { m_Direction = direction; }

	private:
		glm::vec3 m_Direction;
	};
}
