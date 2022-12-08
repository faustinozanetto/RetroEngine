#pragma once

#include "render_pass.h"
#include "renderer/shader/shader.h"

namespace retro::renderer {
	class geometry_pass final : public render_pass
	{
	public:
		geometry_pass();
		~geometry_pass() override;

		void begin_pass() override;

		shared<frame_buffer>& get_pass_output() override;

	private:
		void generate_geometry_fbo();
		void load_shaders();

	private:
		shared<frame_buffer> m_geometry_fbo;
		shared<shader> m_geometry_shader;
	};
}
