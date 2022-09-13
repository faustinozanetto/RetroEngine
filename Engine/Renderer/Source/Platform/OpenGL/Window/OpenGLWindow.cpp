#include "pch.h"

#include "OpenGLWindow.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro {
	OpenGLWindow::OpenGLWindow(const FWindowSpecification& specification) {
		// Update window specification.
		m_WindowSpecification.windowTitle = specification.windowTitle;
		m_WindowSpecification.width = specification.width;
		m_WindowSpecification.height = specification.height;
		m_WindowSpecification.vSync = specification.vSync;
		// Initialize.
		if (!InitializeWindow())
		{
			Logger::Error("Initialized | Could not initialize window.");
		}
	}

	OpenGLWindow::~OpenGLWindow() {

	}

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
		Logger::Info("Window VSync: " + GetWindowSpecification().vSync ? "Enabled" : "Disabled");
		Logger::Line();

		// Setup window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Creating the window
		m_OpenGLWindow = glfwCreateWindow(m_WindowSpecification.width,
										  m_WindowSpecification.height, m_WindowSpecification.windowTitle.c_str(),
										  nullptr, nullptr);
		if (!m_OpenGLWindow)
		{
			Logger::Error("Initialization | Failed to create OpenGL Window.");
			return false;
		}

		// Set user pointer, used in callbacks.
		glfwSetWindowUserPointer(m_OpenGLWindow, &m_WindowSpecification);
		
		// Create Rendering Context.
		const auto rendererContext = RendererContext::Create(m_OpenGLWindow);
		rendererContext->Initialize();
		// Set context to renderer.
		Renderer::SetRendererContext(rendererContext.get());

		// Handle Initial VSync.
		SetEnableVSync(m_WindowSpecification.vSync);

		return true;
	}

	void* OpenGLWindow::GetNativeWindow() const
	{
		return m_OpenGLWindow;
	}

	void OpenGLWindow::WindowLoop()
	{
		glfwSwapBuffers(m_OpenGLWindow);
		Renderer::SetClearColor({ 1.0f, 1.0f, 0.25f, 1.0f });
		Renderer::ClearScreen();
	}

	void OpenGLWindow::SetEnableVSync(bool useVSync)
	{
		// OpenGL Vsync implementation.
		if (useVSync) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		m_WindowSpecification.vSync = useVSync;
	}
}