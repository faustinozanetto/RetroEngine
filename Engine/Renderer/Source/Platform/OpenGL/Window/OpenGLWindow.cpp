#include "pch.h"

#include "OpenGLWindow.h"

namespace Retro {
	OpenGLWindow::OpenGLWindow(const FWindowSpecification& specification) {
		// Update window specification.
		m_WindowSpecification = m_WindowSpecification;
		// Initialize.
		if (!InitializeWindow())
		{
			Logger::Error("Initialized | Could not initialize window.");
		}
	}

	OpenGLWindow::~OpenGLWindow() {

	};

	bool OpenGLWindow::InitializeWindow()
	{
		// Initializing GLFW
		uint32_t glfwState = glfwInit();
		if (glfwState == GLFW_FALSE) {
			return false;
		}
		Logger::Info("Initialization | GLFW Success");
		Logger::Line();
		// Create OpenGL Window.
		Logger::Info("Initialization | Creating OpenGL Window");
		Logger::Info("Window Title: " + GetWindowSpecification().windowTitle);
		Logger::Info("Window Width: " + std::to_string(GetWindowSpecification().width));
		Logger::Info("Window Height: " + std::to_string(GetWindowSpecification().height));
		Logger::Line();

		// Setup window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Creating the window
		m_OpenGLWindow = glfwCreateWindow(GetWindowSpecification().width,
										  GetWindowSpecification().height, GetWindowSpecification().windowTitle.c_str(),
										  nullptr, nullptr);
		if (!m_OpenGLWindow)
		{
			Logger::Error("Initialization | Failed to create OpenGL Window.");
			return false;
		}

		// Set user pointer, used in callbacks.
		glfwSetWindowUserPointer(m_OpenGLWindow, &m_WindowSpecification);
		// Set glfw context
		glfwMakeContextCurrent(m_OpenGLWindow);
		// Glad initialize
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		return true;
	}
}