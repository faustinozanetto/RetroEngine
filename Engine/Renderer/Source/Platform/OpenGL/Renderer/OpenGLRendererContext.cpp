#include "pch.h"

#include "Platform/OpenGL/Renderer/OpenGLRendererContext.h"

namespace Retro::Renderer {
	OpenGLRendererContext::OpenGLRendererContext(GLFWwindow* windowHandle)
	{
		RETRO_ASSERT(windowHandle, "OpenGLRendererContext::OpenGLRendererContext | Window handle invalid!.")
		m_WindowHandle = windowHandle;
		Logger::Info("OpenGLRendererContext::OpenGLRendererContext | Created renderer context.");
	}

	void OpenGLRendererContext::Initialize()
	{
		Logger::Info("OpenGLRendererContext::OpenGLRendererContext | Initializing renderer context.");
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

	void OpenGLRendererContext::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
}