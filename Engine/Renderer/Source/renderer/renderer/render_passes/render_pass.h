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

		uint32_t get_pass_input(int index) const { return m_pass_inputs.at(index); }
		void set_pass_input(int index, uint32_t input_handle) { m_pass_inputs[index] = input_handle; }

	protected:
		bool m_enabled = true;
		std::map<int, uint32_t> m_pass_inputs;
	};
}