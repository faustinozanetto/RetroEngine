#pragma once

#include "core/base.h"
#include "glm/glm.hpp"

namespace Retro::Renderer
{
	enum class renderer_api_type
	{
		none = 0,
		open_gl = 1
	};

	enum class renderer_mode
	{
		normal = 0,
		wireframe = 1,
		point = 2,
	};

	enum class renderer_state
	{
		depth_test
	};

	class renderer_api
	{
	public:
		/* Destructor */
		virtual ~renderer_api() = default;

		/* Methods */
		virtual bool initialize() = 0;
		virtual void set_clear_color(glm::vec4 color) = 0;
		virtual void set_renderer_mode(renderer_mode renderer_mode) = 0;
		virtual void set_renderer_state(renderer_state renderer_state, bool enabled) = 0;
		virtual void clear_screen() = 0;
		virtual void process_rendereable(int size) = 0;
		virtual void bind_texture(uint32_t texture_handle, uint32_t texture_slot = 0) = 0;
		virtual double get_time() = 0;

		/* Getters */
		static renderer_api_type get_renderer_api_type();

		/* Instantiate */
		static unique<renderer_api> create(renderer_api_type renderer_apit_type);

	private:
		static renderer_api_type s_renderer_api_type;
	};
}
