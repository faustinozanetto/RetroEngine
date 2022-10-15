#pragma once

#include "Renderer/Lighting/Light.h"

namespace Retro::Renderer {
	class PointLight : public Light {
	public:
		PointLight();
		~PointLight() = default;

		const float& GetConstant() const { return m_Constant; }
		void SetConstant(float constant) { m_Constant = constant; }

		const float& GetLinear() const { return m_Linear; }
		void SetLinear(float linear) { m_Linear = linear; }

		const float& GetQuadratic() const { return m_Quadratic; }
		void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }

	private:
		float m_Constant = 1.0f;
		float m_Linear = 0.09f;
		float m_Quadratic = 0.032f;
	};
}