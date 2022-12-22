#include "pch.h"

#include "open_gl_renderer_context.h"
#include "core/assert.h"

namespace retro::renderer
{
	open_gl_renderer_context::open_gl_renderer_context(GLFWwindow* window_handle)
	{
		RETRO_CORE_ASSERT(window_handle, "open_gl_renderer_context::open_gl_renderer_context | Window handle invalid!.")
			m_window_handle = window_handle;
		logger::info("open_gl_renderer_context::open_gl_renderer_context | Created renderer context.");
	}

	void open_gl_renderer_context::initialize()
	{
		logger::info("open_gl_renderer_context::initialize | Initializing renderer context.");
		// Set glfw context
		glfwMakeContextCurrent(m_window_handle);
		// Glad initialize
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			logger::error("open_gl_renderer_context::initialize | An error occurred while trying to initialize GLAD");
		}
	}

	void open_gl_renderer_context::swap_buffers()
	{
		glfwSwapBuffers(m_window_handle);
	}

	void open_gl_renderer_context::poll_input()
	{
		glfwPollEvents();
	}

	void open_gl_renderer_context::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	bool open_gl_renderer_context::should_close()
	{
		return glfwWindowShouldClose(m_window_handle);
	}
}