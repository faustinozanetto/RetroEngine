#pragma once

#include "core/input/input_key.h"

namespace retro::input
{
	class input_manager
	{
	public:
		static bool is_key_pressed(key_code key);
	};
}
