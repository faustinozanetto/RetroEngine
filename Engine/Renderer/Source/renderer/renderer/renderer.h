#pragma once

#include <queue>

#include "core/base.h"
#include "render_command.h"
#include "renderer/renderer/renderer_api.h"
#include "renderer/renderer/renderer_context.h"
#include "renderer/window/window.h"
#include "glm/glm.hpp"

namespace retro::renderer
{
	class renderer
	{
	public:
		/* Methods */
		static bool initialize(renderer_api_type renderer_api_type, const window& window);

		static void set_clear_color(glm::vec4 color);
		static void clear_screen();
		static void swap_buffers();
		static void poll_input();
		static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void set_renderer_mode(renderer_mode renderer_mode);
		static void set_renderer_state(renderer_state renderer_state, bool enabled);
		static void bind_texture(uint32_t texture_handle, uint32_t texture_slot = 0);
		static bool should_close();

		static void begin();
		static void end();

		static void submit_command(const render_command& command);

		/* Getters */
		static renderer_api_type get_renderer_api_type();
		static double get_time();

	private:
		static void process_render_command(const render_command& command);

		static unique<renderer_api> s_renderer_api;
		static unique<renderer_context> s_renderer_context;
		static std::queue<render_command> s_command_queue;
	};
}
