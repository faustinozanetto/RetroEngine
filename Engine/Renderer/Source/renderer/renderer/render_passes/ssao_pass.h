#pragma once

#include "render_pass.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/shader/shader.h"
#include "renderer/texture/texture_2d.h"

namespace retro::renderer
{
	struct ssao_parameters
	{
		float radius = 0.5f;
		float bias = 0.02f;
		int samples_count = 64;
		int noise_texture_size = 4;
	};
	class ssao_pass : public render_pass
	{
	public:
		ssao_pass();
		~ssao_pass() override;

		void begin_pass() override;
		void resize(uint32_t width, uint32_t height) override;
		void generate_sample_kernel();
		void generate_noise_texture();

		unsigned int get_ssao_result() { return ssaoColorBufferBlur; }
		unsigned int get_ssao() { return ssaoColorBuffer; }

		ssao_parameters& get_ssao_parameters() { return m_ssao_parameters; }

	private:
		void load_shaders();
		void generate_vao();
		void generate_frame_buffers();

	public:
		shared<frame_buffer>& get_pass_output() override;

	private:
		shared<frame_buffer> m_ssao_frame_buffer;
		shared<frame_buffer> m_ssao_blur_frame_buffer;
		shared<texture_2d> m_noise_texture;

		unsigned int ssaoFBO;
		unsigned int ssaoBlurFBO;
		unsigned int ssaoColorBuffer;
		unsigned int ssaoColorBufferBlur;

		shared<shader> m_ssao_shader;
		shared<shader> m_ssao_blur_shader;

		shared<vertex_array_buffer> m_ssao_vao;

		std::vector<glm::vec3> m_ssao_kernel;

		ssao_parameters m_ssao_parameters;
	};
}
