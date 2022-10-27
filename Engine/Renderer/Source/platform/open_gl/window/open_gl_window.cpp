#include "pch.h"

#include "open_gl_window.h"

#include "core/base.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	open_gl_window::open_gl_window(const window_specification& window_specification)
	{
		logger::line();
		// Update window specification.
		m_window_specification.window_title = window_specification.window_title;
		m_window_specification.width = window_specification.width;
		m_window_specification.height = window_specification.height;
		m_window_specification.v_sync = window_specification.v_sync;
		// Initialize.
		if (!initialize_window())
		{
			logger::error("Initialized | Could not initialize window.");
		}
	}

	open_gl_window::~open_gl_window()
	{
		glfwDestroyWindow(m_open_gl_window);
		glfwTerminate();
	}

	bool open_gl_window::initialize_window()
	{
		// Initializing GLFW
		int success = glfwInit();
		RETRO_CORE_ASSERT(success, "Could not initialize GLFW.");
		logger::info("Initialization | GLFW Success");

		// Create OpenGL Window.
		logger::info("Initialization | Creating OpenGL Window");
		logger::info("Window Title: " + m_window_specification.window_title);
		logger::info("Window Width: " + std::to_string(m_window_specification.width));
		logger::info("Window Height: " + std::to_string(m_window_specification.height));
		const auto vSyncEnabledLog = m_window_specification.v_sync ? "Enabled" : "Disabled";
		logger::info("Window VSync: " + std::to_string(*vSyncEnabledLog));
		logger::line();

		// Setup window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 8);

		// Creating the window
		m_open_gl_window = glfwCreateWindow(m_window_specification.width,
			m_window_specification.height, m_window_specification.window_title.c_str(),
		                                  nullptr,
		                                  nullptr);

		// Set user pointer, used in callbacks.
		glfwSetWindowUserPointer(m_open_gl_window, &m_window_specification);

		// Handle Initial VSync.
		set_vsync_enabled(true);

		// Setup callbacks.
		setup_window_callbacks();
		logger::line();
		return true;
	}

	void* open_gl_window::get_native_window() const
	{
		return m_open_gl_window;
	}

	void open_gl_window::set_vsync_enabled(bool useVSync)
	{
		// OpenGL Vsync implementation.
		glfwSwapInterval(useVSync ? 1 : 0);
		m_window_specification.v_sync = useVSync;
	}

	void open_gl_window::set_window_title(const std::string& title)
	{
		glfwSetWindowTitle(m_open_gl_window, title.c_str());
		m_window_specification.window_title = title;
	}

	void open_gl_window::setup_window_callbacks()
	{
		glfwSetWindowSizeCallback(m_open_gl_window, [](GLFWwindow* window, int width, int height)
		{
			window_specification& windowSpecification = *static_cast<window_specification*>(
			glfwGetWindowUserPointer(window));
			windowSpecification.width = width;
			windowSpecification.height = height;

			std::stringstream ss;
			ss << "WindowResizeEvent: " << width << ", " << height;
			renderer::set_viewport(0, 0, width, height);
			logger::info(ss.str());
		});
	}
}
