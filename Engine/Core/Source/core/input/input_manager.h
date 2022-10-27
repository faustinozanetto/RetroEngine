#pragma once

#include "core/input/input_key.h"

namespace Retro::Input
{
	class input_manager
	{
	public:
		static bool is_key_pressed(key_code key);
	};
}
