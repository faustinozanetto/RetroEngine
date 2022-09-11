#pragma once

#include "Window/Window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Retro::Renderer {
	class OpenGLWindow : public Window {
	public:
		/* Constructor & Destructor */
		OpenGLWindow(const FWindowSpecification& specification);
		~OpenGLWindow() override;

	private:
		bool InitializeWindow();

	private:
		GLFWwindow* m_OpenGLWindow;
	};

}