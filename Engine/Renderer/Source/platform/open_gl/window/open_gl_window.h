#pragma once

#include "renderer/window/window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace retro::renderer
{
	class open_gl_window : public window
	{
	public:
		/* Constructor & Destructor */
		open_gl_window() = default;
		open_gl_window(const window_specification& window_specification);
		~open_gl_window() override;

		/* Getters */
		void* get_native_window() const override;

		/* Methods */
		void set_vsync_enabled(bool useVSync) override;
		void set_window_title(const std::string& title) override;
		void setup_window_callbacks() override;

	private:
		bool initialize_window();

	private:
		GLFWwindow* m_open_gl_window{};
	};
}
