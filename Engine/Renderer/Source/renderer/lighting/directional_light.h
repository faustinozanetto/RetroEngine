#pragma once

#include "renderer/lighting/light.h"

namespace retro::renderer
{
    class directional_light : public light
    {
    public:
        directional_light();
        ~directional_light() = default;

        glm::vec3& get_direction() { return m_direction; }
        void set_direction(const glm::vec3& direction) { m_direction = direction; }

    private:
        glm::vec3 m_direction = {-4.0f, -6.28f, 0.0};
    };
}
