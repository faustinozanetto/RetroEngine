#pragma once

#include "renderer/lighting/light.h"

namespace retro::renderer
{
	class point_light : public light
	{
	public:
		point_light();
		~point_light() override;

		const float& get_constant() const { return m_constant; }
		void set_constant(float constant) { m_constant = constant; }

		const float& get_linear() const { return m_linear; }
		void set_linear(float linear) { m_linear = linear; }

		const float& get_quadratic() const { return m_quadratic; }
		void set_quadratic(float quadratic) { m_quadratic = quadratic; }

	private:
		float m_constant = 1.0f;
		float m_linear = 0.09f;
		float m_quadratic = 0.032f;
	};
}
