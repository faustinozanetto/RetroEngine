#include "pch.h"

#include "Platform/OpenGL/Renderer/OpenGLRendererContext.h"

namespace Retro::Renderer {
	OpenGLRendererContext::OpenGLRendererContext(GLFWwindow* windowHandle)
	{
		RETRO_ASSERT(windowHandle, "OpenGLRendererContext::OpenGLRendererContext | Window handle invalid!.")
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