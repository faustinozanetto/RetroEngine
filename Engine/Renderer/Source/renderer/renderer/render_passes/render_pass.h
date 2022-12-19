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
		virtual void resize(uint32_t width, uint32_t height) = 0;

		bool render_pass_enabled() const { return m_enabled; };
		void set_render_pass_enabled(bool enabled) { m_enabled = enabled; }

	protected:
		bool m_enabled = true;
	};
}