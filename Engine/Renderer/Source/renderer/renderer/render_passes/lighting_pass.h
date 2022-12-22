#pragma once

#include "renderer/shader/shader.h"

#include "render_pass.h"

namespace retro::renderer {
	class lighting_pass final : public render_pass
	{
	public:
		lighting_pass();
		~lighting_pass() override;

		void begin_pass() override;
		void resize(uint32_t width, uint32_t height) override;

		shared<frame_buffer>& get_pass_output() override;

		void initialize_inputs();
		void bind_inputs();

	private:
		void generate_frame_buffer();
		void load_shaders();

	private:
		shared<shader> m_lighting_shader;
		shared<frame_buffer> m_lighthing_frame_buffer;
	};
}