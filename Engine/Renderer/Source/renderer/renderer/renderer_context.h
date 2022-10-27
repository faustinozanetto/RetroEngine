#pragma once

#include "core/base.h"

namespace retro::renderer
{
	class renderer_context
	{
	public:
		/* Destructor */
		virtual ~renderer_context() = default;

		/* Methods */
		virtual void initialize() = 0;
		virtual void swap_buffers() = 0;
		virtual void poll_input() = 0;
		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual bool should_close() = 0;

		/* Instantiate */
		static unique<renderer_context> create(void * window_handle);
	};
}
