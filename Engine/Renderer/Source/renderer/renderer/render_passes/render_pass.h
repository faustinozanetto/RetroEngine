#pragma once

#include "core/base.h"
#include "renderer/buffers/fbo/frame_buffer.h"

namespace retro::renderer
{
	class render_pass
	{
	public:
		virtual ~render_pass() = default;

		virtual shared<frame_buffer>& get_pass_output() = 0;
		virtual void begin_pass() = 0;
	};
}