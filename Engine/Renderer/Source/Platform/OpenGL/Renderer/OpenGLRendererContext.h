#pragma once

#include "Renderer/Renderer/RendererContext.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace Retro::Renderer {
	class OpenGLRendererContext : public RendererContext {
	public:
		/* Constructor */
		OpenGLRendererContext(GLFWwindow* windowHandle);

		/* Methods */
		void Initialize() override;
		void SwapBuffers() override;
		void PollInput() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}