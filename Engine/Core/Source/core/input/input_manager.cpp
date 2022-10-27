#include "pch.h"

#include "input_manager.h"
#include "core/application/retro_application.h"

#include "../../ThirdParty/GLFW/include/GLFW/glfw3.h"

namespace retro::input
{
	bool input_manager::is_key_pressed(key_code key)
	{
		auto window = static_cast<GLFWwindow *>(retro_application::get_application().get_window()->get_native_window());
		int state = glfwGetKey(window, key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
}
