#pragma once

#include <glm/vec2.hpp>

#include "core/input/input_key.h"
#include "core/utils/singleton.h"

namespace retro::input
{
	class input_manager : public singleton<input_manager>
	{
	public:
		static bool is_key_pressed(key_code key);
		static bool is_mouse_button_pressed(mouse_key_code mouse_key);

		static glm::vec2 get_mouse_location();
	};
}
