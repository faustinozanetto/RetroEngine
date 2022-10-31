#pragma once

#include "core/base.h"
#include <glm/glm.hpp>

namespace retro::editor
{
    class editor_interface_utils
    {
    public:
        static void draw_property(const std::string& name);
        static void draw_property(const std::string& name, const std::string& content);
        static bool draw_property(const std::string& name, bool& value);
        static bool draw_property(const std::string& name, int& value, int min, int max, int step = 1);
        static bool draw_property(const std::string& name, float& value, float min, float max, float step = 0.1f);
        static void draw_property(const std::string& name, glm::vec3& value, float min, float max, float step = 0.1f);
        static bool draw_property(const std::string& name, glm::vec3& value, bool color = true);
        static bool draw_property(const std::string& name, glm::vec4& value, bool color = true);
    };
}
