#pragma once

#include "InputKey.h"

namespace Retro::Input
{
	class InputManager
	{
	public:
		static bool IsKeyPressed(KeyCode key);
	};
}
