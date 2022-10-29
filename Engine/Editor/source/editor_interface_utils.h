#pragma once

#include "core/base.h"
#include <glm/glm.hpp>

namespace retro::editor
{
    class editor_interface_utils
    {
    public:
        static bool draw_property(const std::string& name, glm::vec3& value, float min, float max, float step = 0.1f);
    };
}
