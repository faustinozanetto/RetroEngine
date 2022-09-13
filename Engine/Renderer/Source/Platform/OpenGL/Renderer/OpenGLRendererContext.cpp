#include "pch.h"

#include "Platform/OpenGL/Renderer/OpenGLRendererContext.h"

namespace Retro {
	OpenGLRendererContext::OpenGLRendererContext(GLFWwindow* windowHandle)
	{
		m_WindowHandle = windowHandle;
	}

	void OpenGLRendererContext::Initialize()
	{
		// Set glfw context
		glfwMakeContextCurrent(m_WindowHandle);
		// Glad initialize
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	}

	void OpenGLRendererContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void OpenGLRendererContext::PollInput()
	{
		glfwPollEvents();
	}
}