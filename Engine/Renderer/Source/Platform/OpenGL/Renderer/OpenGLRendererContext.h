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
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}