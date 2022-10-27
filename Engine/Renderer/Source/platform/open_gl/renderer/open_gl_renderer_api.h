#pragma once

#include "renderer/renderer/renderer_api.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace retro::renderer
{
	class open_gl_renderer_api : public renderer_api
	{
	public:
		/* Constructor & Destructor */
		open_gl_renderer_api();
		~open_gl_renderer_api() override;

		/* Methods */
		bool initialize() override;
		void set_clear_color(glm::vec4 color) override;
		void set_renderer_mode(renderer_mode renderer_mode) override;
		void set_renderer_state(renderer_state renderer_state, bool enabled) override;
		void clear_screen() override;
		void process_rendereable(int size) override;
		void bind_texture(uint32_t texture_handle, uint32_t texture_slot) override;
		double get_time() override;

		static GLenum get_renderer_state_to_open_gl(renderer_state renderer_state);
	};
}
