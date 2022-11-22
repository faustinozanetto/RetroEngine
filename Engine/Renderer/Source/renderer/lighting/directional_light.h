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
        void set_direction(float azimuth, float elevation);

    private:
        glm::vec3 m_direction = {0.0f, 0.0f, 0.0};
    };
}
