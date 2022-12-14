#pragma once

#include "core/base.h"
#include <glm/glm.hpp>

#include "renderer/materials/material.h"
#include "renderer/texture/texture_2d.h"

namespace retro::editor
{
	class editor_interface_utils
	{
	public:
		static void draw_property(const std::string& name);
		static void draw_property(const std::string& name, const std::string& content);
		static bool draw_property(const std::string& name, bool& value);
		static bool draw_property(const std::string& name, int& value, int min, int max);
		static bool draw_property(const std::string& name, float& value, float min, float max, float step = 0.1f);
		static bool draw_property(const std::string& name, glm::vec2& value, float min, float max, float step = 0.1f);
		static void draw_property(const std::string& name, glm::vec3& value, float min, float max, float step = 0.1f);
		static bool draw_property(const std::string& name, glm::vec3& value, bool color = true);
		static bool draw_property(const std::string& name, glm::vec4& value, bool color = true);
		static bool draw_property(const shared<renderer::texture_2d>& texture);
		static bool draw_property(renderer::material_texture& material_texture);
	};
}
