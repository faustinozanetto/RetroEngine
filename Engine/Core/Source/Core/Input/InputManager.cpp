#include "pch.h"
#include "InputManager.h"
#include "Core/Application/RetroApplication.h"
#include "../../ThirdParty/GLFW/include/GLFW/glfw3.h"

namespace Retro::Input
{
	bool InputManager::IsKeyPressed(KeyCode key)
	{
		auto window = static_cast<GLFWwindow*>(RetroApplication::GetApplication().GetWindow()->GetNativeWindow());
		int state = glfwGetKey(window, key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
}
