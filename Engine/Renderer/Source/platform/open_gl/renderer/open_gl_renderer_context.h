#pragma once

#include "platform/open_gl/window/open_gl_window.h"
#include "renderer/renderer/renderer_context.h"

namespace retro::renderer
{
	class open_gl_renderer_context : public renderer_context
	{
	public:
		/* Constructor */
		open_gl_renderer_context(GLFWwindow* window_handle);

		/* Methods */
		void initialize() override;
		void swap_buffers() override;
		void poll_input() override;
		void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		bool should_close() override;

	private:
		GLFWwindow* m_window_handle;
	};
}
