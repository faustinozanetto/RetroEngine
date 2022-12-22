#pragma once

#include "render_pass.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/shader/shader.h"

namespace retro::renderer {
	class fxaa_pass : public render_pass
	{
	public:
		fxaa_pass();
		~fxaa_pass() override;

		void begin_pass() override;
		void resize(uint32_t width, uint32_t height) override;

		shared<frame_buffer>& get_pass_output() override;

	private:
		void load_shaders();
		void generate_vao();
		void generate_frame_buffers();

	private:
		shared<frame_buffer> m_fxaa_frame_buffer;
		shared<shader> m_fxaa_shader;

		shared<vertex_array_buffer> m_fxaa_vao;
	};
}
